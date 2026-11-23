/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** search of the possible duplicates among the present songs, from what
** is written about them: same core of title, same version (a remix is
** not the original), an artist in common (uploader included), close
** durations. The pairs are then gathered in groups (dup_groups.c) and
** the sound is compared afterwards (dup_audio.c), only for them.
*/

#include "mp3player.h"

static int same_song(dup_info_t const *a, dup_info_t const *b)
{
    long diff = a->song->duration_ms - b->song->duration_ms;

    if (a->version != b->version || a->core[0] == '\0'
        || strcmp(a->core, b->core) != 0)
        return 0;
    if (a->song->duration_ms > 0 && b->song->duration_ms > 0
        && (diff > DUP_MAX_DIFF_MS || diff < -DUP_MAX_DIFF_MS))
        return 0;
    return dup_artists_meet(a->artists, b->artists);
}

static void fill_info(library_t *lib, dup_info_t *info, song_t *song)
{
    char prefix[NAME_LEN];
    char clean[NAME_LEN];

    info->song = song;
    dup_core(song_title(song), info->core, prefix);
    dup_artists(lib, song, prefix, info->artists);
    dup_clean(song_title(song), clean, NAME_LEN);
    info->version = dup_version(clean);
}

static void add_pair(dup_list_t *list, song_t *a, song_t *b)
{
    dup_pair_t *bigger = NULL;

    if (list->nb_pairs % 16 == 0) {
        bigger = realloc(list->pairs, sizeof(dup_pair_t)
            * (list->nb_pairs + 16));
        if (!bigger)
            return;
        list->pairs = bigger;
    }
    list->pairs[list->nb_pairs].a = a;
    list->pairs[list->nb_pairs].b = b;
    list->nb_pairs++;
}

/*
** the pairs of the song i with the songs after it
*/
static void pairs_of(library_t *lib, dup_info_t *infos, int i,
    dup_list_t *list)
{
    song_t *a = infos[i].song;
    song_t *b = NULL;
    long since = list->since;

    for (int j = i + 1; j < lib->nb; j++) {
        b = infos[j].song;
        if ((since > 0 && a->first_seen < since && b->first_seen < since)
            || not_dup_find(lib, a, b) || !same_song(&infos[i], &infos[j]))
            continue;
        add_pair(list, a, b);
    }
}

/*
** the groups found replace those of the list; returns 84 on a failed
** allocation
*/
int dup_find(library_t *lib, dup_list_t *list)
{
    dup_info_t *infos = malloc(sizeof(dup_info_t) * (lib->nb + 1));

    free(list->groups);
    list->groups = NULL;
    list->nb = 0;
    list->nb_pairs = 0;
    if (!infos)
        return 84;
    for (int i = 0; i < lib->nb; i++)
        fill_info(lib, &infos[i], lib->present[i]);
    for (int i = 0; i < lib->nb; i++)
        pairs_of(lib, infos, i, list);
    free(infos);
    return dup_group_pairs(list);
}
