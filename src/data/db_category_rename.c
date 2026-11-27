/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a category of the author renamed (category_custom.c): its name is
** changed in the category column of battles.log and battles_undone.log,
** written again in <log>.tmp, then renamed (never half written)
*/

#include "mp3player.h"

/*
** line: timestamp, kind, ranking, category, ... (a "reset" line has no
** category); writes it with the new name if it names the old one
*/
static void rename_line(FILE *out, char const *line, char const *old,
    char const *name)
{
    char const *start = line;
    size_t len = 0;

    for (int tabs = 0; tabs < 3 && start; tabs++) {
        start = strchr(start, '\t');
        start = start ? start + 1 : NULL;
    }
    if (line[0] == '#' || !start || strncmp(line + strcspn(line, "\t"),
            "\treset\t", 7) == 0) {
        fputs(line, out);
        return;
    }
    len = strcspn(start, "\t\n");
    if (len != strlen(old) || strncmp(start, old, len) != 0) {
        fputs(line, out);
        return;
    }
    fwrite(line, 1, start - line, out);
    fputs(name, out);
    fputs(start + len, out);
}

static void log_rename(char const *path, char const *old, char const *name)
{
    char tmp[PATH_LEN];
    FILE *in = fopen(path, "r");
    FILE *out = NULL;
    char *line = NULL;
    size_t size = 0;

    if (!in)
        return;
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    out = fopen(tmp, "w");
    if (!out) {
        fclose(in);
        return;
    }
    while (getline(&line, &size, in) > 0)
        rename_line(out, line, old, name);
    free(line);
    fclose(in);
    if (fclose(out) == 0)
        rename(tmp, path);
}

void db_category_rename(char const *old, char const *name)
{
    log_rename(BATTLES_LOG, old, name);
    log_rename(BATTLES_UNDONE, old, name);
}
