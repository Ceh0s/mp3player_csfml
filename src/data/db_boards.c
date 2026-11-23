/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes boards.tsv and board_rules.tsv (the rankings and
** their rules); the general ranking (id 0) is never written
*/

#include "mp3player.h"

void db_parse_board(library_t *lib, char **fields, int nb)
{
    if (nb != 2 || atoi(fields[0]) <= 0 || fields[1][0] == '\0')
        return;
    board_add(lib, atoi(fields[0]), fields[1]);
}

static int type_from_name(char const *name)
{
    for (int i = 0; i < NB_RULE_TYPES; i++)
        if (strcmp(rule_type_name(i), name) == 0)
            return i;
    return -1;
}

/*
** fields: board_id, mode (include / exclude), type, target
*/
void db_parse_rule(library_t *lib, char **fields, int nb)
{
    board_t *board = NULL;
    int type = 0;

    if (nb < 3)
        return;
    board = board_by_id(lib, atoi(fields[0]));
    type = type_from_name(fields[2]);
    if (!board || board->id == BOARD_GENERAL || type < 0)
        return;
    rule_add(board, strcmp(fields[1], "include") == 0, type,
        nb > 3 ? atoi(fields[3]) : 0);
}

static int save_rules(library_t *lib)
{
    FILE *file = tsv_create(RULES_DB, RULES_COLUMNS);

    if (!file)
        return 84;
    for (board_t *board = lib->boards; board; board = board->next) {
        if (board->id <= BOARD_GENERAL)
            continue;
        for (rule_t *rule = board->rules; rule; rule = rule->next)
            fprintf(file, "%d\t%s\t%s\t%d\n", board->id,
                rule->include ? "include" : "exclude",
                rule_type_name(rule->type), rule->target);
    }
    return tsv_commit(file, RULES_DB);
}

int db_save_boards(library_t *lib)
{
    FILE *file = tsv_create(BOARDS_DB, BOARDS_COLUMNS);

    if (!file)
        return 84;
    for (board_t *board = lib->boards; board; board = board->next)
        if (board->id > BOARD_GENERAL)
            fprintf(file, "%d\t%s\n", board->id, board->name);
    if (tsv_commit(file, BOARDS_DB) == 84)
        return 84;
    return save_rules(lib);
}
