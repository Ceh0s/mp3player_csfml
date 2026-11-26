/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** value from 0 to 100 of a song or an artist in a category, for the bars
** of the stats (stat_panel.c): the share of the others it is above, in
** a ranking, among those with at least one battle in it
*/

#include "mp3player.h"

/*
** a song counts if it is present (its ratings are on its resolved id)
*/
static int counted(library_t *lib, elo_t const *elo, arena_t const *arena)
{
    song_t *song = NULL;

    if (elo->kind != arena->kind || elo->board != arena->board
        || elo->category != arena->category || elo->battles == 0)
        return 0;
    if (elo->kind == KIND_ARTIST)
        return 1;
    song = song_by_id(lib, elo->subject);
    return song && song->present;
}

/*
** arena : kind, ranking and category; subject : resolved id; -1 without
** any battle in it (ex aequo count for half, 50 if it is the only one)
*/
int elo_percent(library_t *lib, arena_t const *arena, int subject)
{
    elo_t *own = elo_find(lib, arena, subject);
    int others = 0;
    double above = 0;

    if (!own || own->battles == 0)
        return -1;
    for (elo_t *elo = lib->elo; elo; elo = elo->next) {
        if (elo == own || !counted(lib, elo, arena))
            continue;
        others++;
        if (own->rating > elo->rating)
            above++;
        if (own->rating == elo->rating)
            above += 0.5;
    }
    if (others == 0)
        return 50;
    return (int)(100 * above / others + 0.5);
}
