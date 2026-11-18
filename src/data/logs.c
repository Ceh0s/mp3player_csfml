/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** append-only history files (listens, edits, merges)
*/

#include "mp3player.h"

void log_line(char const *path, char const *columns, char const *line)
{
    FILE *file = NULL;

    tsv_touch(path, columns);
    file = fopen(path, "a");
    if (!file) {
        fprintf(stderr, "Cannot write %s\n", path);
        return;
    }
    fprintf(file, "%ld\t%s\n", (long)time(NULL), line);
    fclose(file);
}
