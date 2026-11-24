/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a song being forgotten (song_forget.c) is taken out of the logs: its
** listens (listens.log) and its battles (battles.log, the Elo are then
** replayed without them), and its images are removed. Each log is
** written again in <log>.tmp, then renamed (never half written).
*/

#include "mp3player.h"

/*
** listens.log: timestamp, song id, ms, end
*/
static int names_listen(char *line, int id)
{
    char *fields[8];
    int nb = split_tab(line, fields, 8);

    return nb >= 2 && atoi(fields[1]) == id;
}

/*
** battles.log: timestamp, kind, ranking, category, winner, loser...
** (a "reset" line names no song)
*/
static int names_battle(char *line, int id)
{
    char *fields[10];
    int nb = split_tab(line, fields, 10);

    if (nb < 6 || strcmp(fields[1], "song") != 0)
        return 0;
    return atoi(fields[4]) == id || atoi(fields[5]) == id;
}

/*
** the lines naming the song are left out; the header is kept
*/
static void filter_lines(FILE *in, FILE *out, int id,
    int (*names)(char *, int))
{
    char *line = NULL;
    char *copy = NULL;
    size_t size = 0;

    while (getline(&line, &size, in) > 0) {
        copy = strdup(line);
        if (!copy)
            break;
        copy[strcspn(copy, "\n")] = '\0';
        if (line[0] == '#' || !names(copy, id))
            fputs(line, out);
        free(copy);
    }
    free(line);
}

static void log_filter(char const *path, int id, int (*names)(char *, int))
{
    char tmp[PATH_LEN];
    FILE *in = fopen(path, "r");
    FILE *out = NULL;

    if (!in)
        return;
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    out = fopen(tmp, "w");
    if (!out) {
        fclose(in);
        return;
    }
    filter_lines(in, out, id, names);
    fclose(in);
    if (fclose(out) == 0)
        rename(tmp, path);
}

void song_forget_logs(song_t *song)
{
    rank_t rank = {song, NULL, 0, 0, 0};
    char path[PATH_LEN];

    log_filter(LISTENS_LOG, song->id, &names_listen);
    log_filter(BATTLES_LOG, song->id, &names_battle);
    image_path(path, &rank, 0);
    remove(path);
    image_path(path, &rank, 1);
    remove(path);
}
