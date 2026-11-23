/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** order between the ex aequo of the ranking shown: the order saved for
** the ranking and kind (tie_order.c), the others after it by title
*/

#include "mp3player.h"
#include <strings.h>

static int compare_names(void const *a, void const *b)
{
    return strcasecmp(rank_name(&((ranked_t const *)a)->rank),
        rank_name(&((ranked_t const *)b)->rank));
}

static int compare_ties(void const *a, void const *b)
{
    long first = ((ranked_t const *)a)->tie;
    long second = ((ranked_t const *)b)->tie;

    return (first > second) - (first < second);
}

/*
** position of the resolved id in the saved order, -1 if not in it
*/
static long tie_position(tie_order_t const *tie, int const *resolved,
    int id)
{
    for (int i = 0; tie && i < tie->nb; i++)
        if (resolved[i] == id)
            return i;
    return -1;
}

/*
** items are sorted by their tie key when it returns
*/
void ranking_tie_keys(app_t *app, ranked_t *items, int nb)
{
    tie_order_t *tie = tie_find(app->lib, app->arena.board,
        app->arena.kind);
    int saved = tie ? tie->nb : 0;
    int *resolved = malloc(sizeof(int) * (saved + 1));
    long pos = 0;

    if (!resolved)
        return;
    for (int i = 0; i < saved; i++)
        resolved[i] = subject_resolve(app->lib, app->arena.kind,
            tie->ids[i]);
    qsort(items, nb, sizeof(ranked_t), &compare_names);
    for (int i = 0; i < nb; i++) {
        pos = tie_position(tie, resolved, rank_id(&items[i].rank));
        items[i].tie = pos >= 0 ? pos : saved + i;
    }
    free(resolved);
    qsort(items, nb, sizeof(ranked_t), &compare_ties);
}

/*
** the order shown in the "Départage" tab becomes the saved one
*/
void ranking_save_order(app_t *app)
{
    ranking_t *ranking = app->ranking;
    int *ids = malloc(sizeof(int) * (ranking->nb + 1));

    if (!ids)
        return;
    for (int i = 0; i < ranking->nb; i++)
        ids[i] = rank_id(&ranking->order[i]);
    tie_set(app->lib, &app->arena, ids, ranking->nb);
    free(ids);
    db_save_ties(app->lib);
    ranking_build(app);
}
