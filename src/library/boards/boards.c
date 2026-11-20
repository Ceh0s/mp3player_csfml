/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** rankings (boards): creation, search, removal; the general one
** (id 0, every song) is always the first of the list
*/

#include "mp3player.h"

/*
** id 0 is the general ranking, otherwise the board is appended at the
** end of the list; returns NULL if the id is already used
*/
board_t *board_add(library_t *lib, int id, char const *name)
{
    board_t **last = &lib->boards;
    board_t *board = NULL;

    if (board_by_id(lib, id))
        return NULL;
    board = calloc(1, sizeof(board_t));
    if (!board)
        return NULL;
    board->id = id;
    board->name = strdup(name);
    if (!board->name) {
        free(board);
        return NULL;
    }
    while (*last)
        last = &(*last)->next;
    *last = board;
    if (id >= lib->next_board)
        lib->next_board = id + 1;
    return board;
}

board_t *board_by_id(library_t *lib, int id)
{
    for (board_t *board = lib->boards; board; board = board->next)
        if (board->id == id)
            return board;
    return NULL;
}

int board_general_create(library_t *lib)
{
    board_t *board = board_add(lib, BOARD_GENERAL, "Général");

    if (!board)
        return 84;
    return rule_add(board, 1, RULE_ALL, 0);
}

static void board_free(board_t *board)
{
    rule_t *next = NULL;

    while (board->rules) {
        next = board->rules->next;
        free(board->rules);
        board->rules = next;
    }
    free(board->name);
    free(board);
}

/*
** board = NULL frees all of them; the general one cannot be removed
** alone. Its battles stay in battles.log, they are then ignored.
*/
void board_remove(library_t *lib, board_t *board)
{
    board_t **cur = &lib->boards;
    board_t *old = NULL;

    while (*cur) {
        if (board && (*cur != board || board->id == BOARD_GENERAL)) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        board_free(old);
    }
}
