/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes rejected.tsv: the videos refused in the download
** screen because of their duration (id, duration, date, title, refused
** while in the allowed duration; the old files have no last column)
*/

#include "mp3player.h"

void db_parse_rejected(library_t *lib, char **fields, int nb)
{
    rejected_t *rej = NULL;

    if ((nb != 4 && nb != 5) || fields[0][0] == '\0')
        return;
    rej = rejected_add(lib, fields[0], fields[3], atoi(fields[1]));
    if (!rej)
        return;
    rej->date = atol(fields[2]);
    rej->in_range = nb == 5 && atoi(fields[4]) != 0;
}

int db_save_rejected(library_t *lib)
{
    FILE *file = tsv_create(REJECTED_DB, REJECTED_COLUMNS);

    if (!file)
        return 84;
    for (rejected_t *rej = lib->rejected; rej; rej = rej->next)
        fprintf(file, "%s\t%d\t%ld\t%s\t%d\n", rej->id, rej->duration,
            rej->date, rej->title, rej->in_range);
    return tsv_commit(file, REJECTED_DB);
}
