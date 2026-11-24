/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** order of the artists of a song (the order of its links): links_reverse
** keeps the order of song_artists.tsv when it is read (each link is
** added in front); before that fix the order flipped at each launch,
** so links_order_repair puts the artists of each song back in the order
** of its tag, once (settings.tsv: links_ordered)
*/

#include "mp3player.h"
#include <strings.h>

void links_reverse(library_t *lib)
{
    link_t *done = NULL;
    link_t *next = NULL;

    while (lib->links) {
        next = lib->links->next;
        lib->links->next = done;
        done = lib->links;
        lib->links = next;
    }
    lib->links = done;
}

/*
** place of the name of the artist in the tag of the song (the end if
** not in it): its role first, so the main artists stay first
*/
static long link_key(library_t *lib, song_t const *song, link_t const *link)
{
    artist_t *artist = artist_by_id(lib, link->artist_id);
    char const *found = NULL;
    long pos = 100000;

    if (artist)
        found = strcasestr(song->src_artists, artist->name);
    if (found)
        pos = found - song->src_artists;
    return link->role * 1000000L + pos;
}

/*
** the links of the song are taken out of the list (in order, nb at
** most LINK_SORT_MAX) and sorted by link_key (insertion, stable)
*/
static int take_links(library_t *lib, song_t const *song, link_t **out)
{
    link_t **cur = &lib->links;
    link_t *item = NULL;
    int nb = 0;
    int j = 0;

    while (*cur && nb < LINK_SORT_MAX) {
        if ((*cur)->song_id != song->id) {
            cur = &(*cur)->next;
            continue;
        }
        item = *cur;
        *cur = item->next;
        j = nb;
        while (j > 0 && link_key(lib, song, out[j - 1])
            > link_key(lib, song, item)) {
            out[j] = out[j - 1];
            j--;
        }
        out[j] = item;
        nb++;
    }
    return nb;
}

static void order_song(library_t *lib, song_t const *song)
{
    link_t *links[LINK_SORT_MAX];
    int nb = take_links(lib, song, links);

    for (int i = nb - 1; i >= 0; i--) {
        links[i]->next = lib->links;
        lib->links = links[i];
    }
}

void links_order_repair(library_t *lib)
{
    if (lib->settings.links_ordered)
        return;
    for (song_t *song = lib->songs; song; song = song->next)
        if (song->src_artists && song->merged_into == 0)
            order_song(lib, song);
    lib->settings.links_ordered = 1;
    db_save_settings(lib);
}
