/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** songs of a playlist in the order chosen for it (sorts.c): the value
** of each song is computed once, then a merge sort keeps the order of
** the playlist between equal songs. Used by the list of the library and
** by the queue of the player, so both always follow the same order.
*/

#include "mp3player.h"
#include <ctype.h>
#include <strings.h>

/*
** "2018" -> 20180000, "2018-05-12" or "20180512" -> 20180512, -1 if the
** tag has no year
*/
static long release_date(char const *year)
{
    long date = 0;
    int digits = 0;

    for (int i = 0; year && year[i] && digits < 8; i++) {
        if (!isdigit((unsigned char)year[i]))
            continue;
        date = date * 10 + year[i] - '0';
        digits++;
    }
    if (digits < 4)
        return -1;
    for (; digits < 8; digits++)
        date *= 10;
    return date;
}

static long item_number(library_t *lib, view_sort_t const *sort,
    song_t *song, int native)
{
    if (sort->key == SORT_DURATION)
        return song->duration_ms > 0 ? song->duration_ms : -1;
    if (sort->key == SORT_LISTEN)
        return song_total_listen(lib, song);
    if (sort->key == SORT_ADDED)
        return song->first_seen > 0 ? song->first_seen : -1;
    if (sort->key == SORT_RELEASE)
        return release_date(song->year);
    return native;
}

static void fill_item(library_t *lib, view_sort_t const *sort,
    sort_item_t *item, int native)
{
    song_t *song = item->song;

    item->text = NULL;
    if (sort->key == SORT_TITLE)
        item->text = song_title(song);
    if (sort->key == SORT_ARTIST)
        item->text = song->artists;
    item->num = item_number(lib, sort, song, native);
    if (sort->key == SORT_TITLE || sort->key == SORT_ARTIST)
        item->known = item->text && item->text[0] != '\0';
    else
        item->known = item->num >= 0;
}

/*
** < 0 if a comes before b; a missing value is last in both directions
*/
static int compare(sort_item_t const *a, sort_item_t const *b, int desc)
{
    int diff = 0;

    if (a->known != b->known)
        return a->known ? -1 : 1;
    if (!a->known)
        return 0;
    if (a->text && b->text)
        diff = strcasecmp(a->text, b->text);
    else
        diff = (a->num > b->num) - (a->num < b->num);
    return desc ? -diff : diff;
}

/*
** the two sorted halves of items are merged in tmp, then copied back;
** on equal values the first half wins, so the sort is stable
*/
static void merge_halves(sort_item_t *items, sort_item_t *tmp, int nb,
    int desc)
{
    int half = nb / 2;
    int i = 0;
    int j = half;
    int left = 0;

    for (int k = 0; k < nb; k++) {
        left = j >= nb
            || (i < half && compare(&items[j], &items[i], desc) >= 0);
        if (left) {
            tmp[k] = items[i];
            i++;
        } else {
            tmp[k] = items[j];
            j++;
        }
    }
    memcpy(items, tmp, sizeof(sort_item_t) * nb);
}

static void merge_sort(sort_item_t *items, sort_item_t *tmp, int nb,
    int desc)
{
    if (nb < 2)
        return;
    merge_sort(items, tmp, nb / 2, desc);
    merge_sort(items + nb / 2, tmp, nb - nb / 2, desc);
    merge_halves(items, tmp, nb, desc);
}

/*
** the custom ids are resolved once, so that a merged song keeps the
** place of the song it was merged into
*/
static void sort_view(library_t *lib, view_sort_t const *sort,
    song_t **view, int nb)
{
    sort_item_t *items = malloc(sizeof(sort_item_t) * (nb * 2 + 1));
    song_t **custom = malloc(sizeof(song_t *) * (sort->nb_custom + 1));

    for (int i = 0; items && custom && i < sort->nb_custom; i++)
        custom[i] = song_resolve(lib, song_by_id(lib, sort->custom[i]));
    for (int i = 0; items && custom && i < nb; i++) {
        items[i].song = view[i];
        fill_item(lib, sort, &items[i], i);
        if (sort->key == SORT_CUSTOM)
            items[i].num = custom_position(sort, custom, view[i], i);
    }
    if (items && custom) {
        merge_sort(items, items + nb, nb, sort->desc);
        for (int i = 0; i < nb; i++)
            view[i] = items[i].song;
    }
    free(items);
    free(custom);
}

int playlist_sorted_view(library_t *lib, playlist_t const *playlist,
    song_t **view)
{
    int nb = playlist_view(lib, playlist, view);
    view_sort_t *sort = sort_get(lib, playlist ? playlist->id : 0);

    if (sort && nb > 1)
        sort_view(lib, sort, view, nb);
    return nb;
}
