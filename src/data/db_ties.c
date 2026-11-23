/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes tie_orders.tsv: the order chosen between the ex
** aequo of each ranking (board, kind, raw id, position; in order)
*/

#include "mp3player.h"

/*
** fields: board, kind, id, position; the lines come in order
*/
void db_parse_tie(library_t *lib, char **fields, int nb)
{
    int kind = 0;

    if (nb != 4)
        return;
    kind = kind_find(fields[1]);
    if (kind < 0)
        return;
    tie_append(lib, atoi(fields[0]), kind, atoi(fields[2]));
}

int db_save_ties(library_t *lib)
{
    FILE *file = tsv_create(TIES_DB, TIES_COLUMNS);

    if (!file)
        return 84;
    for (tie_order_t *tie = lib->ties; tie; tie = tie->next)
        for (int i = 0; i < tie->nb; i++)
            fprintf(file, "%d\t%s\t%d\t%d\n", tie->board,
                kind_name(tie->kind), tie->ids[i], i + 1);
    return tsv_commit(file, TIES_DB);
}
