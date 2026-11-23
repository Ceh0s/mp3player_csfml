/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** order chosen by the user between the songs (or artists) of a ranking
** that have the same Elo, the same % of victories and the same number
** of victories; one per ranking and kind, raw ids (resolved when used)
*/

#include "mp3player.h"

tie_order_t *tie_find(library_t *lib, int board, int kind)
{
    for (tie_order_t *tie = lib->ties; tie; tie = tie->next)
        if (tie->board == board && tie->kind == kind)
            return tie;
    return NULL;
}

static tie_order_t *tie_get(library_t *lib, int board, int kind)
{
    tie_order_t *tie = tie_find(lib, board, kind);

    if (tie)
        return tie;
    tie = calloc(1, sizeof(tie_order_t));
    if (!tie)
        return NULL;
    tie->board = board;
    tie->kind = kind;
    tie->next = lib->ties;
    lib->ties = tie;
    return tie;
}

/*
** at load, the ids come one by one in their order
*/
int tie_append(library_t *lib, int board, int kind, int id)
{
    tie_order_t *tie = tie_get(lib, board, kind);
    int *bigger = tie ? realloc(tie->ids, sizeof(int) * (tie->nb + 1))
        : NULL;

    if (!bigger)
        return 84;
    tie->ids = bigger;
    tie->ids[tie->nb] = id;
    tie->nb++;
    return 0;
}

static int in_list(int const *ids, int nb, int id)
{
    for (int i = 0; i < nb; i++)
        if (ids[i] == id)
            return 1;
    return 0;
}

/*
** the nb ids become the order; the ids kept before that are not among
** them (songs not present now) stay at the end, nothing is lost
*/
int tie_set(library_t *lib, arena_t const *arena, int const *ids, int nb)
{
    tie_order_t *tie = tie_get(lib, arena->board, arena->kind);
    int *order = tie ? malloc(sizeof(int) * (nb + tie->nb + 1)) : NULL;
    int count = nb;

    if (!order)
        return 84;
    memcpy(order, ids, sizeof(int) * nb);
    for (int i = 0; i < tie->nb; i++)
        if (!in_list(ids, nb, tie->ids[i])) {
            order[count] = tie->ids[i];
            count++;
        }
    free(tie->ids);
    tie->ids = order;
    tie->nb = count;
    return 0;
}

void ties_free(library_t *lib)
{
    tie_order_t *next = NULL;

    while (lib->ties) {
        next = lib->ties->next;
        free(lib->ties->ids);
        free(lib->ties);
        lib->ties = next;
    }
}
