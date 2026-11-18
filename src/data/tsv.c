/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** generic reading and writing of the .tsv save files
** (one line per record, fields separated by tabs, '#' lines are headers)
*/

#include "mp3player.h"
#include <sys/stat.h>

static void read_line(char *line, tsv_fn_t fn, library_t *lib,
    int *version)
{
    char *fields[TSV_MAX_FIELDS];
    int nb = 0;

    if (line[0] == '#') {
        if (*version == 0 && strncmp(line, "#v", 2) == 0)
            *version = atoi(line + 2);
        return;
    }
    if (line[0] == '\n' || line[0] == '\0')
        return;
    nb = split_tab(line, fields, TSV_MAX_FIELDS);
    fn(lib, fields, nb);
}

/*
** calls fn on every record of the file, returns the version of the file
*/
int tsv_read(char const *path, tsv_fn_t fn, library_t *lib)
{
    FILE *file = fopen(path, "r");
    char *line = NULL;
    size_t size = 0;
    int version = 0;

    if (!file)
        return 0;
    while (getline(&line, &size, file) != -1)
        read_line(line, fn, lib, &version);
    free(line);
    fclose(file);
    return version;
}

/*
** Files are written to "<path>.tmp" then renamed by tsv_commit:
** if the program crashes while saving, the previous save is intact.
*/
FILE *tsv_create(char const *path, char const *columns)
{
    char tmp[256];
    FILE *file = NULL;

    mkdir(DATA_DIR, 0755);
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    file = fopen(tmp, "w");
    if (!file) {
        fprintf(stderr, "Cannot write %s\n", tmp);
        return NULL;
    }
    fprintf(file, "#v2\n#%s\n", columns);
    return file;
}

int tsv_commit(FILE *file, char const *path)
{
    char tmp[256];

    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    if (fclose(file) != 0 || rename(tmp, path) != 0) {
        fprintf(stderr, "Cannot save %s\n", path);
        return 84;
    }
    return 0;
}

void tsv_touch(char const *path, char const *columns)
{
    FILE *file = NULL;

    mkdir(DATA_DIR, 0755);
    if (access(path, F_OK) == 0)
        return;
    file = fopen(path, "w");
    if (!file)
        return;
    fprintf(file, "#v2\n#%s\n", columns);
    fclose(file);
}
