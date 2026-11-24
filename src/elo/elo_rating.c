/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** standard Elo: E_a = 1 / (1 + 10^((R_b - R_a) / 400)),
** R_a' = R_a + K * (S_a - E_a), K = 40 for the first 10 battles of a
** song (or artist) in a category of a ranking, then 20
*/

#include "mp3player.h"
#include <math.h>

elo_t *elo_find(library_t *lib, arena_t const *arena, int subject)
{
    for (elo_t *elo = lib->elo; elo; elo = elo->next)
        if (elo->subject == subject && elo->kind == arena->kind
            && elo->board == arena->board
            && elo->category == arena->category)
            return elo;
    return NULL;
}

/*
** a song or artist without any battle in the arena starts at 1500
*/
elo_t *elo_get(library_t *lib, arena_t const *arena, int subject)
{
    elo_t *elo = elo_find(lib, arena, subject);

    if (elo)
        return elo;
    elo = calloc(1, sizeof(elo_t));
    if (!elo)
        return NULL;
    elo->kind = arena->kind;
    elo->board = arena->board;
    elo->subject = subject;
    elo->category = arena->category;
    elo->rating = ELO_START;
    elo->next = lib->elo;
    lib->elo = elo;
    return elo;
}

static double expected(double rating, double other)
{
    return 1.0 / (1.0 + pow(10.0, (other - rating) / 400.0));
}

/*
** the two ratings after a battle (the battles before it give K); also
** used by the replay of the history (elo_history.c)
*/
void elo_update(double *ratings, int const *battles)
{
    double win_expected = expected(ratings[0], ratings[1]);
    double lose_expected = expected(ratings[1], ratings[0]);

    ratings[0] += (battles[0] < ELO_NEW_BATTLES ? ELO_K_NEW : ELO_K_OLD)
        * (1.0 - win_expected);
    ratings[1] += (battles[1] < ELO_NEW_BATTLES ? ELO_K_NEW : ELO_K_OLD)
        * (0.0 - lose_expected);
}

/*
** winner and loser are resolved ids (subject_resolve)
*/
void elo_battle(library_t *lib, arena_t const *arena, int winner,
    int loser)
{
    elo_t *win = NULL;
    elo_t *lose = NULL;
    double ratings[2];
    int battles[2];

    if (winner == loser)
        return;
    win = elo_get(lib, arena, winner);
    lose = elo_get(lib, arena, loser);
    if (!win || !lose)
        return;
    ratings[0] = win->rating;
    ratings[1] = lose->rating;
    battles[0] = win->battles;
    battles[1] = lose->battles;
    elo_update(ratings, battles);
    win->rating = ratings[0];
    lose->rating = ratings[1];
    win->battles++;
    lose->battles++;
    win->wins++;
}

void elo_clear(library_t *lib)
{
    elo_t *next = NULL;

    while (lib->elo) {
        next = lib->elo->next;
        free(lib->elo);
        lib->elo = next;
    }
}
