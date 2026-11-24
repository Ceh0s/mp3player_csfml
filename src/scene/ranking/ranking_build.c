/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the songs or artists of the arena, best first: every one of
** the ranking, even without a battle (1500). Same Elo (rounded as it
** is shown) : the best % of victories first, then the most victories,
** then the order chosen by the user (ranking_order.c)
*/

#include "mp3player.h"
#include <math.h>

/*
** the best % of victories first (0 % without any battle)
*/
static int compare_ratio(rank_t const *first, rank_t const *second)
{
    double a = first->battles ? (double)first->wins / first->battles : 0;
    double b = second->battles ? (double)second->wins / second->battles : 0;

    return (a < b) - (a > b);
}

static int compare_ranks(void const *a, void const *b)
{
    ranked_t const *first = a;
    ranked_t const *second = b;
    long one = lround(first->rank.rating);
    long two = lround(second->rank.rating);
    int ratio = compare_ratio(&first->rank, &second->rank);

    if (one != two)
        return (one < two) - (one > two);
    if (ratio != 0)
        return ratio;
    if (first->rank.wins != second->rank.wins)
        return (first->rank.wins < second->rank.wins)
            - (first->rank.wins > second->rank.wins);
    return (first->tie > second->tie) - (first->tie < second->tie);
}

static void ranking_clear(ranking_t *ranking)
{
    free(ranking->ranks);
    free(ranking->order);
    free(ranking->tiers);
    ranking->ranks = NULL;
    ranking->order = NULL;
    ranking->tiers = NULL;
    ranking->nb = 0;
}

/*
** order : the songs in the order between ex aequo, ranks : best first
*/
static void fill(ranking_t *ranking, ranked_t *items, int nb)
{
    ranking->ranks = malloc(sizeof(rank_t) * (nb + 1));
    ranking->order = malloc(sizeof(rank_t) * (nb + 1));
    ranking->tiers = malloc(sizeof(int) * (nb + 1));
    if (!ranking->ranks || !ranking->order || !ranking->tiers) {
        ranking_clear(ranking);
        return;
    }
    for (int i = 0; i < nb; i++)
        ranking->order[i] = items[i].rank;
    qsort(items, nb, sizeof(ranked_t), &compare_ranks);
    for (int i = 0; i < nb; i++)
        ranking->ranks[i] = items[i].rank;
    ranking->nb = nb;
    ranking_assign_tiers(ranking);
}

static void score_pool(app_t *app, pool_t const *pool)
{
    ranked_t *items = malloc(sizeof(ranked_t) * (pool->nb + 1));

    if (!items)
        return;
    for (int i = 0; i < pool->nb; i++) {
        items[i].rank = pool->items[i];
        elo_score(app->lib, &app->arena, &items[i].rank);
    }
    ranking_tie_keys(app, items, pool->nb);
    fill(app->ranking, items, pool->nb);
    free(items);
}

void ranking_build(app_t *app)
{
    pool_t pool = {NULL, 0};

    ranking_clear(app->ranking);
    app->ranking->history_dirty = 1;
    if (!board_by_id(app->lib, app->arena.board))
        app->arena.board = BOARD_GENERAL;
    if (pool_build(app->lib, &app->arena, &pool) == 84)
        return;
    score_pool(app, &pool);
    free(pool.items);
    ranking_scroll(app, 0);
}
