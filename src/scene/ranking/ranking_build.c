/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the ranked songs or artists of the arena, best rating first:
** only those of the ranking having at least one battle in the category
** (in one of the real categories for TOTAL)
*/

#include "mp3player.h"

static int compare_rating(void const *a, void const *b)
{
    double first = ((rank_t const *)a)->rating;
    double second = ((rank_t const *)b)->rating;

    return (first < second) - (first > second);
}

void ranking_build(app_t *app)
{
    ranking_t *ranking = app->ranking;
    pool_t pool = {NULL, 0};

    free(ranking->ranks);
    ranking->ranks = NULL;
    ranking->nb = 0;
    ranking->scroll = 0;
    if (!board_by_id(app->lib, app->arena.board))
        app->arena.board = BOARD_GENERAL;
    if (pool_build(app->lib, &app->arena, &pool) == 84)
        return;
    for (int i = 0; i < pool.nb; i++) {
        elo_score(app->lib, &app->arena, &pool.items[i]);
        if (pool.items[i].battles == 0)
            continue;
        pool.items[ranking->nb] = pool.items[i];
        ranking->nb++;
    }
    ranking->ranks = pool.items;
    qsort(ranking->ranks, ranking->nb, sizeof(rank_t), &compare_rating);
}
