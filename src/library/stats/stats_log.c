/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads listens.log for the statistics screen: one session per line
** (date, raw id of the song, time listened, how it ended), the only
** listening time that has a date
*/

#include "mp3player.h"

static int parse(char *line, listen_t *out)
{
    char *fields[4];

    if (line[0] == '#' || split_tab(line, fields, 4) != 4)
        return 0;
    out->date = atol(fields[0]);
    out->song_id = atoi(fields[1]);
    out->ms = atol(fields[2]);
    out->skip = strcmp(fields[3], "skip") == 0;
    return out->song_id > 0;
}

/*
** the array grows twice bigger when it is full; returns 84 if the
** memory is missing (the sessions read so far are kept)
*/
static int grow(listen_t **log, int *size)
{
    int bigger = *size == 0 ? 256 : *size * 2;
    listen_t *more = realloc(*log, sizeof(listen_t) * bigger);

    if (!more)
        return 84;
    *log = more;
    *size = bigger;
    return 0;
}

/*
** returns the number of sessions read in *out (to free)
*/
int stats_log_load(listen_t **out)
{
    FILE *file = fopen(LISTENS_LOG, "r");
    char *line = NULL;
    size_t len = 0;
    int nb = 0;
    int size = 0;

    *out = NULL;
    if (!file)
        return 0;
    while (getline(&line, &len, file) != -1) {
        if (nb == size && grow(out, &size) == 84)
            break;
        nb += parse(line, &(*out)[nb]);
    }
    free(line);
    fclose(file);
    return nb;
}
