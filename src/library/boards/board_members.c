/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** which songs are in a ranking: a present song is in it if at least one
** rule includes it and no rule excludes it. The rules are evaluated each
** time, so a new song of an excluded artist is excluded too.
*/

#include "mp3player.h"

/*
** every role counts: a featuring of an excluded artist is excluded
*/
static int song_has_artist(library_t *lib, song_t const *song,
    int artist_id)
{
    for (link_t *link = lib->links; link; link = link->next)
        if (link->song_id == song->id && link->artist_id == artist_id)
            return 1;
    return 0;
}

static int rule_matches(library_t *lib, rule_t const *rule, song_t *song)
{
    playlist_t *playlist = NULL;

    if (rule->type == RULE_ALL)
        return 1;
    if (rule->type == RULE_ARTIST)
        return song_has_artist(lib, song, rule->target);
    if (rule->type == RULE_SONG)
        return song_resolve(lib, song_by_id(lib, rule->target)) == song;
    playlist = playlist_by_id(lib, rule->target);
    return playlist && playlist_has_song(lib, playlist, song);
}

/*
** song is a resolved song
*/
int board_has_song(library_t *lib, int board_id, song_t *song)
{
    board_t *board = board_by_id(lib, board_id);
    int in = 0;

    if (!board)
        return 0;
    for (rule_t *rule = board->rules; rule; rule = rule->next) {
        if (!rule_matches(lib, rule, song))
            continue;
        if (!rule->include)
            return 0;
        in = 1;
    }
    return in;
}
