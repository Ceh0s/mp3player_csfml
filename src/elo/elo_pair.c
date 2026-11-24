/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** chooses the two songs (or artists) of a battle, among the pool:
** first one of those with the fewest battles in the arena,
** then an opponent among the 3 closest ratings
*/

#include "mp3player.h"
#include <math.h>

static int compare_gap(void const *a, void const *b)
{
    double first = ((rank_t const *)a)->rating;
    double second = ((rank_t const *)b)->rating;

    return (first > second) - (first < second);
}

static int pick_first(rank_t const *ranks, int nb)
{
    int fewest = ranks[0].battles;
    int count = 0;
    int target = 0;

    for (int i = 1; i < nb; i++)
        if (ranks[i].battles < fewest)
            fewest = ranks[i].battles;
    for (int i = 0; i < nb; i++)
        count += ranks[i].battles == fewest;
    target = rand() % count;
    for (int i = 0; i < nb; i++) {
        if (ranks[i].battles != fewest)
            continue;
        if (target == 0)
            return i;
        target--;
    }
    return 0;
}

/*
** number of opponents to draw from: the 3 closest ratings, and all
** those as close as the third one (most songs are still at 1500: the
** tie would else always give the first ones of the library, in the
** alphabetical order)
*/
static int closest(rank_t const *ranks, int nb)
{
    int choices = nb < 3 ? nb : 3;
    double limit = ranks[choices - 1].rating;

    while (choices < nb && ranks[choices].rating <= limit)
        choices++;
    return choices;
}

/*
** the first one is removed from the array, the others are sorted by
** the gap between their rating and its rating; the side of each one is
** random too, so none is always on the left
*/
static void draw_pair(rank_t *ranks, int nb, rank_t *pair)
{
    int first = pick_first(ranks, nb);
    int side = rand() % 2;
    rank_t chosen = ranks[first];

    pair[side] = chosen;
    ranks[first] = ranks[nb - 1];
    for (int i = 0; i < nb - 1; i++)
        ranks[i].rating = fabs(ranks[i].rating - chosen.rating);
    qsort(ranks, nb - 1, sizeof(rank_t), &compare_gap);
    pair[1 - side] = ranks[rand() % closest(ranks, nb - 1)];
}

static int same(rank_t const *a, rank_t const *b)
{
    return a->song == b->song && a->artist == b->artist;
}

static int same_pair(rank_t const *pair, rank_t const *previous)
{
    return (same(&pair[0], &previous[0]) && same(&pair[1], &previous[1]))
        || (same(&pair[0], &previous[1]) && same(&pair[1], &previous[0]));
}

/*
** pair holds the previous pair, which is not drawn again if possible;
** returns 0 if there are fewer than two songs (or artists) in the pool
*/
int elo_pick_pair(library_t *lib, arena_t const *arena, pool_t *pool,
    rank_t *pair)
{
    rank_t previous[2] = {pair[0], pair[1]};
    rank_t *ranks = NULL;

    if (pool->nb < 2)
        return 0;
    ranks = malloc(sizeof(rank_t) * pool->nb);
    if (!ranks)
        return 0;
    for (int i = 0; i < pool->nb; i++)
        elo_score(lib, arena, &pool->items[i]);
    for (int try = 0; try < 5; try++) {
        memcpy(ranks, pool->items, sizeof(rank_t) * pool->nb);
        draw_pair(ranks, pool->nb, pair);
        if (!same_pair(pair, previous))
            break;
    }
    free(ranks);
    return 1;
}
