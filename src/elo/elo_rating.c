/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** standard Elo: E_a = 1 / (1 + 10^((R_b - R_a) / 400)),
** R_a' = R_a + K * (S_a - E_a), K = 40 for the first 10 battles of a
** song in a category, then 20
*/

#include "mp3player.h"
#include <math.h>

elo_t *elo_find(library_t *lib, int song_id, int category)
{
    for (elo_t *elo = lib->elo; elo; elo = elo->next)
        if (elo->song_id == song_id && elo->category == category)
            return elo;
    return NULL;
}

/*
** a song without any battle in the category starts at 1500
*/
elo_t *elo_get(library_t *lib, int song_id, int category)
{
    elo_t *elo = elo_find(lib, song_id, category);

    if (elo)
        return elo;
    elo = calloc(1, sizeof(elo_t));
    if (!elo)
        return NULL;
    elo->song_id = song_id;
    elo->category = category;
    elo->rating = ELO_START;
    elo->next = lib->elo;
    lib->elo = elo;
    return elo;
}

static double expected(double rating, double other)
{
    return 1.0 / (1.0 + pow(10.0, (other - rating) / 400.0));
}

static double k_factor(elo_t const *elo)
{
    return elo->battles < ELO_NEW_BATTLES ? ELO_K_NEW : ELO_K_OLD;
}

/*
** winner and loser are resolved song ids (song_resolve)
*/
void elo_battle(library_t *lib, int winner, int loser, int category)
{
    elo_t *win = NULL;
    elo_t *lose = NULL;
    double win_expected = 0;
    double lose_expected = 0;

    if (winner == loser)
        return;
    win = elo_get(lib, winner, category);
    lose = elo_get(lib, loser, category);
    if (!win || !lose)
        return;
    win_expected = expected(win->rating, lose->rating);
    lose_expected = expected(lose->rating, win->rating);
    win->rating += k_factor(win) * (1.0 - win_expected);
    lose->rating += k_factor(lose) * (0.0 - lose_expected);
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
