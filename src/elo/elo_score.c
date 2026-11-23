/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** score of a song or artist in a category, or in the virtual TOTAL
** category (sum of the ratings of the rated categories, "Tout
** confondu" and the fun ones excluded), and what identifies the
** subject of a score
*/

#include "mp3player.h"

/*
** id of the song or artist of the score
*/
int rank_id(rank_t const *rank)
{
    if (rank->song)
        return rank->song->id;
    return rank->artist ? rank->artist->id : 0;
}

char const *rank_name(rank_t const *rank)
{
    if (rank->song)
        return song_title(rank->song);
    return rank->artist ? rank->artist->name : "?";
}

/*
** id as it must be counted: a song merged into another one counts as
** that one; 0 if the song or artist does not exist
*/
int subject_resolve(library_t *lib, elo_kind_t kind, int id)
{
    song_t *song = NULL;

    if (kind == KIND_ARTIST)
        return artist_by_id(lib, id) ? id : 0;
    song = song_resolve(lib, song_by_id(lib, id));
    return song ? song->id : 0;
}

static void add_category(library_t *lib, arena_t const *arena,
    int category, rank_t *out)
{
    arena_t one = {arena->kind, arena->board, category};
    elo_t *elo = elo_find(lib, &one, rank_id(out));

    if (!elo) {
        out->rating += ELO_START;
        return;
    }
    out->rating += elo->rating;
    out->battles += elo->battles;
    out->wins += elo->wins;
}

/*
** out->song or out->artist says whose score it is
*/
void elo_score(library_t *lib, arena_t const *arena, rank_t *out)
{
    out->rating = 0;
    out->battles = 0;
    out->wins = 0;
    if (arena->category != CAT_TOTAL) {
        add_category(lib, arena, arena->category, out);
        return;
    }
    for (int i = 0; i < NB_CATEGORIES; i++)
        if (category_rated(i))
            add_category(lib, arena, i, out);
}
