/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** score of a song in a category, or in the virtual TOTAL category
** (sum of the ratings of the real categories, GLOBAL excluded)
*/

#include "mp3player.h"

static void add_category(library_t *lib, int song_id, int category,
    rank_t *out)
{
    elo_t *elo = elo_find(lib, song_id, category);

    if (!elo) {
        out->rating += ELO_START;
        return;
    }
    out->rating += elo->rating;
    out->battles += elo->battles;
    out->wins += elo->wins;
}

void elo_score(library_t *lib, song_t *song, int category, rank_t *out)
{
    out->song = song;
    out->rating = 0;
    out->battles = 0;
    out->wins = 0;
    if (category != CAT_TOTAL) {
        add_category(lib, song->id, category, out);
        return;
    }
    for (int i = CAT_GLOBAL + 1; i < NB_CATEGORIES; i++)
        add_category(lib, song->id, i, out);
}
