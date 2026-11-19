/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** chooses the two songs of a battle, among the present ones:
** first one of the songs with the fewest battles in the category,
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
** the first song is removed from the array, the others are sorted by
** the gap between their rating and its rating
*/
static song_t *pick_opponent(rank_t *ranks, int nb, int first)
{
    rank_t chosen = ranks[first];
    int choices = nb - 1 < 3 ? nb - 1 : 3;

    ranks[first] = ranks[nb - 1];
    for (int i = 0; i < nb - 1; i++)
        ranks[i].rating = fabs(ranks[i].rating - chosen.rating);
    qsort(ranks, nb - 1, sizeof(rank_t), &compare_gap);
    return ranks[rand() % choices].song;
}

/*
** the side of each song is random too, so none is always on the left
*/
static void draw_pair(library_t *lib, int category, rank_t *ranks,
    song_t **pair)
{
    int first = 0;
    int side = rand() % 2;

    for (int i = 0; i < lib->nb; i++)
        elo_score(lib, lib->playlist[i], category, &ranks[i]);
    first = pick_first(ranks, lib->nb);
    pair[side] = ranks[first].song;
    pair[1 - side] = pick_opponent(ranks, lib->nb, first);
}

static int same_pair(song_t *const *pair, song_t *const *previous)
{
    return (pair[0] == previous[0] && pair[1] == previous[1])
        || (pair[0] == previous[1] && pair[1] == previous[0]);
}

/*
** pair holds the previous pair, which is not drawn again if possible;
** returns 0 if there are fewer than two songs
*/
int elo_pick_pair(library_t *lib, int category, song_t **pair)
{
    song_t *previous[2] = {pair[0], pair[1]};
    rank_t *ranks = NULL;

    if (lib->nb < 2)
        return 0;
    ranks = malloc(sizeof(rank_t) * lib->nb);
    if (!ranks)
        return 0;
    for (int try = 0; try < 5; try++) {
        draw_pair(lib, category, ranks, pair);
        if (!same_pair(pair, previous))
            break;
    }
    free(ranks);
    return 1;
}
