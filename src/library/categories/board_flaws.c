/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** quality or flaw of a category in one ranking only (board_flaws.tsv):
** in the ranking of a playlist of songs unpleasant on purpose, "Agression
** auditive" can count as a quality. Only the rankings set apart are kept,
** the others follow the choice of the category.
*/

#include "mp3player.h"

static board_flaw_t *board_flaw_find(library_t *lib, int board, int stat)
{
    for (board_flaw_t *f = lib->board_flaws; f; f = f->next)
        if (f->board == board && f->stat == stat)
            return f;
    return NULL;
}

/*
** 1 if the category measures a flaw in this ranking
*/
int category_flaw(library_t *lib, int board, int stat)
{
    board_flaw_t *f = NULL;

    if (board != 0)
        f = board_flaw_find(lib, board, stat);
    return f ? f->flaw : lib->cats[stat].flaw;
}

static void board_flaw_remove(library_t *lib, int board, int stat)
{
    board_flaw_t **link = &lib->board_flaws;
    board_flaw_t *f = NULL;

    while (*link) {
        f = *link;
        if ((board < 0 || f->board == board) && f->stat == stat) {
            *link = f->next;
            free(f);
        } else
            link = &f->next;
    }
}

/*
** the same as the category: nothing kept, the ranking follows it
*/
void board_flaw_set(library_t *lib, int board, int stat, int flaw)
{
    board_flaw_t *f = board_flaw_find(lib, board, stat);

    if (board == 0)
        return;
    if (flaw == lib->cats[stat].flaw) {
        board_flaw_remove(lib, board, stat);
        return;
    }
    if (f) {
        f->flaw = flaw;
        return;
    }
    f = calloc(1, sizeof(board_flaw_t));
    if (!f)
        return;
    f->board = board;
    f->stat = stat;
    f->flaw = flaw;
    f->next = lib->board_flaws;
    lib->board_flaws = f;
}

/*
** a category removed: its choices go, the following ones move back
*/
void board_flaws_remove_cat(library_t *lib, int stat)
{
    board_flaw_remove(lib, -1, stat);
    for (board_flaw_t *f = lib->board_flaws; f; f = f->next)
        if (f->stat > stat && f->stat < MAX_CATEGORIES)
            f->stat--;
}

void board_flaws_free(library_t *lib)
{
    board_flaw_t *next = NULL;

    for (board_flaw_t *f = lib->board_flaws; f; f = next) {
        next = f->next;
        free(f);
    }
    lib->board_flaws = NULL;
}
