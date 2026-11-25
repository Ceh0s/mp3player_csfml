/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** detail of the stats of an artist: the durations of the songs present
** where he appears, split by what he did on it (original, remixed by
** someone else, remixer, featuring), and the time listened. Computed from the current links, never stored.
*/

#include "mp3player.h"

int song_has_role(library_t *lib, int song_id, role_t role)
{
    for (link_t *link = lib->links; link; link = link->next)
        if (link->song_id == song_id && link->role == role)
            return 1;
    return 0;
}

artist_part_t song_part(library_t *lib, link_t const *link)
{
    if (link->role == ROLE_REMIX)
        return PART_REMIXER;
    if (link->role == ROLE_FEAT)
        return PART_FEAT;
    if (song_has_role(lib, link->song_id, ROLE_REMIX))
        return PART_REMIXED;
    return PART_ORIGINAL;
}

static void add_song(artist_stats_t *stats, song_t const *song,
    artist_part_t part)
{
    long ms = song->duration_ms > 0 ? song->duration_ms : 0;

    stats->ms[part] += ms;
    stats->titles[part]++;
    stats->total_ms += ms;
    stats->total_titles++;
}

/*
** an artist has only one role per song (see link_add), so each song is
** counted once and the total is the sum of the parts
*/
void artist_stats(library_t *lib, int artist_id, artist_stats_t *stats)
{
    song_t *song = NULL;

    memset(stats, 0, sizeof(artist_stats_t));
    for (link_t *link = lib->links; link; link = link->next) {
        song = song_by_id(lib, link->song_id);
        if (link->artist_id != artist_id || !song || song->merged_into)
            continue;
        stats->listen_ms += song_total_listen(lib, song);
        if (song->present)
            add_song(stats, song, song_part(lib, link));
    }
}
