/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reset of the battle stats: a "reset" line is written in battles.log
** (the history stays, nothing is removed), and the replay forgets the
** ratings it built so far for that ranking and kind (or all of them),
** so they start again from 1500
** line: timestamp, "reset", kind or "all", board or "all"
*/

#include "mp3player.h"

/*
** kind < 0 : every kind; all : every ranking
*/
static int concerned(elo_t const *elo, int kind, int board, int all)
{
    if (kind >= 0 && (int)elo->kind != kind)
        return 0;
    return all || elo->board == board;
}

static void elo_forget(library_t *lib, int kind, int board, int all)
{
    elo_t **link = &lib->elo;
    elo_t *elo = NULL;

    while (*link) {
        elo = *link;
        if (concerned(elo, kind, board, all)) {
            *link = elo->next;
            free(elo);
        } else
            link = &elo->next;
    }
}

/*
** fields[1] is "reset"; returns 1 if the line was a reset
*/
int elo_parse_reset(library_t *lib, char **fields, int nb)
{
    int all = 0;
    int kind = -1;

    if (nb < 4 || strcmp(fields[1], "reset") != 0)
        return 0;
    all = strcmp(fields[3], "all") == 0;
    if (strcmp(fields[2], "all") != 0)
        kind = kind_find(fields[2]);
    if (strcmp(fields[2], "all") == 0 || kind >= 0)
        elo_forget(lib, kind, atoi(fields[3]), all);
    return 1;
}

/*
** all = 1 : every ranking and kind, else the ranking and kind of arena
*/
void elo_reset(library_t *lib, arena_t const *arena, int all)
{
    char line[64];

    if (all)
        snprintf(line, sizeof(line), "reset\tall\tall");
    else
        snprintf(line, sizeof(line), "reset\t%s\t%d",
            kind_name(arena->kind), arena->board);
    log_line(BATTLES_LOG, BATTLES_COLUMNS, line);
    elo_forget(lib, all ? -1 : (int)arena->kind, arena->board, all);
    db_save_elo(lib);
}
