/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes playlist_history.tsv: stays of the songs in the
** playlists (period_t in song.h), a date is in seconds (start: 0 if
** unknown; end: 0 while the song is in the playlist, -1 if unknown)
*/

#include "mp3player.h"

static char const *const origins[NB_ORIGINS] = {"unknown", "auto",
    "import", "hand"};
static char const *const firsts[NB_FIRSTS] = {"auto", "yes", "no"};

/*
** index of name, 0 if it is not one of them
*/
static int name_index(char const *const *names, int nb, char const *name)
{
    for (int i = 0; i < nb; i++)
        if (strcmp(names[i], name) == 0)
            return i;
    return 0;
}

void db_parse_period(library_t *lib, char **fields, int nb)
{
    period_t period = {0};

    if (nb != 6 || atoi(fields[0]) <= 0 || atoi(fields[1]) <= 0)
        return;
    period.playlist = atoi(fields[0]);
    period.song = atoi(fields[1]);
    period.start = atol(fields[2]);
    period.end = atol(fields[3]);
    period.origin = name_index(origins, NB_ORIGINS, fields[4]);
    period.first = name_index(firsts, NB_FIRSTS, fields[5]);
    period_add(lib, &period);
}

int db_save_periods(library_t *lib)
{
    FILE *file = tsv_create(PERIODS_DB, PERIODS_COLUMNS);

    if (!file)
        return 84;
    for (period_t *p = lib->periods; p; p = p->next)
        fprintf(file, "%d\t%d\t%ld\t%ld\t%s\t%s\n", p->playlist, p->song,
            p->start, p->end, origins[p->origin], firsts[p->first]);
    return tsv_commit(file, PERIODS_DB);
}
