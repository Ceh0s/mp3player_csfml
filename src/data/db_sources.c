/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes playlist_sources.tsv: the addresses linked to local
** playlists (playlist id, address, allowed duration, report absents)
*/

#include "mp3player.h"

void db_parse_source(library_t *lib, char **fields, int nb)
{
    source_t *src = NULL;

    if (nb != 5 || !playlist_by_id(lib, atoi(fields[0]))
        || fields[1][0] == '\0')
        return;
    src = source_set(lib, atoi(fields[0]), fields[1]);
    if (!src)
        return;
    src->min_s = atoi(fields[2]);
    src->max_s = atoi(fields[3]);
    src->absent = atoi(fields[4]);
}

int db_save_sources(library_t *lib)
{
    FILE *file = tsv_create(SOURCES_DB, SOURCES_COLUMNS);

    if (!file)
        return 84;
    for (source_t *src = lib->sources; src; src = src->next)
        fprintf(file, "%d\t%s\t%d\t%d\t%d\n", src->playlist_id, src->url,
            src->min_s, src->max_s, src->absent);
    return tsv_commit(file, SOURCES_DB);
}
