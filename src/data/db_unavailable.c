/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes unavailable.tsv: the videos YouTube does not serve
** (id, date, title, reason given by yt-dlp); a line whose reason is no
** longer a lasting one is dropped (the video is tried again)
*/

#include "mp3player.h"

void db_parse_unavailable(library_t *lib, char **fields, int nb)
{
    rejected_t *un = NULL;

    if ((nb != 4 && nb != 5) || fields[0][0] == '\0'
        || !dl_reason_lasting(fields[3]))
        return;
    un = unavailable_add(lib, fields[0], fields[2], fields[3]);
    if (!un)
        return;
    un->date = atol(fields[1]);
    un->archived = nb == 5 && atoi(fields[4]) != 0;
}

int db_save_unavailable(library_t *lib)
{
    FILE *file = tsv_create(UNAVAILABLE_DB, UNAVAILABLE_COLUMNS);

    if (!file)
        return 84;
    for (rejected_t *un = lib->unavailable; un; un = un->next)
        fprintf(file, "%s\t%ld\t%s\t%s\t%d\n", un->id, un->date, un->title,
            un->reason, un->archived);
    return tsv_commit(file, UNAVAILABLE_DB);
}
