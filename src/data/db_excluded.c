/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes excluded.tsv: the videos never to download again
** (id, date, title, archived; the old files have no last column)
*/

#include "mp3player.h"

void db_parse_excluded(library_t *lib, char **fields, int nb)
{
    rejected_t *ex = NULL;

    if ((nb != 3 && nb != 4) || fields[0][0] == '\0')
        return;
    ex = excluded_add(lib, fields[0], fields[2]);
    if (!ex)
        return;
    ex->date = atol(fields[1]);
    ex->archived = nb == 4 && atoi(fields[3]) != 0;
}

int db_save_excluded(library_t *lib)
{
    FILE *file = tsv_create(EXCLUDED_DB, EXCLUDED_COLUMNS);

    if (!file)
        return 84;
    for (rejected_t *ex = lib->excluded; ex; ex = ex->next)
        fprintf(file, "%s\t%ld\t%s\t%d\n", ex->id, ex->date, ex->title,
            ex->archived);
    return tsv_commit(file, EXCLUDED_DB);
}
