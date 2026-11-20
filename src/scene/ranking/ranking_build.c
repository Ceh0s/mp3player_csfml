/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the ranked songs of a category, best rating first: only the
** present songs having at least one battle in it (in one of the real
** categories for TOTAL)
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
    library_t *lib = app->lib;
    rank_t *rank = NULL;

    free(ranking->ranks);
    ranking->nb = 0;
    ranking->scroll = 0;
    ranking->ranks = malloc(sizeof(rank_t) * (lib->nb + 1));
    if (!ranking->ranks)
        return;
    for (int i = 0; i < lib->nb; i++) {
        rank = &ranking->ranks[ranking->nb];
        elo_score(lib, lib->present[i], ranking->category, rank);
        if (rank->battles > 0)
            ranking->nb++;
    }
    qsort(ranking->ranks, ranking->nb, sizeof(rank_t), &compare_rating);
}
