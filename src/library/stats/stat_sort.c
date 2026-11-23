/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** order of the rows of the statistics screen: time listened, sessions,
** skips, Elo or name, the biggest (or Z) first unless ascending
*/

#include "mp3player.h"

static char const *row_name(stat_row_t const *row)
{
    if (row->artist)
        return row->artist->name;
    return song_title(row->song);
}

/*
** < 0 if a is smaller than b, 0 if equal, > 0 if bigger
*/
static int compare(stat_row_t const *a, stat_row_t const *b, int sort)
{
    if (sort == STAT_SORT_PLAYS)
        return (a->plays > b->plays) - (a->plays < b->plays);
    if (sort == STAT_SORT_SKIPS)
        return (a->skips > b->skips) - (a->skips < b->skips);
    if (sort == STAT_SORT_ELO)
        return (a->elo > b->elo) - (a->elo < b->elo);
    if (sort == STAT_SORT_NAME)
        return strcasecmp(row_name(a), row_name(b));
    return (a->ms > b->ms) - (a->ms < b->ms);
}

static int before(stat_row_t const *a, stat_row_t const *b, int sort,
    int ascending)
{
    int diff = compare(a, b, sort);

    return ascending ? diff < 0 : diff > 0;
}

/*
** insertion sort (stable: equal rows keep the order of the playlist)
*/
void stat_sort(stat_row_t *rows, int nb, int sort, int ascending)
{
    stat_row_t row;
    int j = 0;

    for (int i = 1; i < nb; i++) {
        row = rows[i];
        j = i;
        while (j > 0 && before(&row, &rows[j - 1], sort, ascending)) {
            rows[j] = rows[j - 1];
            j--;
        }
        rows[j] = row;
    }
}
