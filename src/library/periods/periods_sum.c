/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** what is said of a song in a playlist, from its stays: how many, since
** when, and if it is there since the beginning of the playlist
*/

#include "mp3player.h"

/*
** the stay going on, NULL if the song is not in the playlist (its own
** id first: the aliases are only looked for if it is not found)
*/
period_t *period_current(library_t *lib, int playlist, song_t const *song)
{
    int ids[SONG_IDS];
    int nb = 0;

    for (period_t *p = lib->periods; p; p = p->next)
        if (p->song == song->id && p->playlist == playlist
            && p->end == PERIOD_OPEN)
            return p;
    nb = song_ids(lib, song, ids);
    for (period_t *p = lib->periods; p && nb > 1; p = p->next)
        if (p->playlist == playlist && p->end == PERIOD_OPEN
            && id_among(p->song, ids, nb))
            return p;
    return NULL;
}

/*
** two stays at the same time (of two songs merged since) count once; a
** lost end is the same as its start
*/
static int overlap(period_t const *a, period_t const *b)
{
    long end_a = a->end == PERIOD_LOST ? a->start : a->end;
    long end_b = b->end == PERIOD_LOST ? b->start : b->end;

    if (a->song == b->song)
        return 0;
    return (b->end == PERIOD_OPEN || a->start <= end_b)
        && (a->end == PERIOD_OPEN || b->start <= end_a);
}

/*
** 1 if a stay met before p in the list is at the same time
*/
static int counted(library_t *lib, period_t const *p, int const *ids, int nb)
{
    for (period_t *q = lib->periods; q != p; q = q->next)
        if (q->playlist == p->playlist && id_among(q->song, ids, nb)
            && overlap(q, p))
            return 1;
    return 0;
}

/*
** oldest date known in the playlist, 0 if none. While some dates of the
** playlist are unknown (it is older than its history), those seen by
** the app do not tell when it began
*/
static long first_date(library_t *lib, int playlist)
{
    long any = 0;
    long sure = 0;
    int unknown = 0;

    for (period_t *p = lib->periods; p; p = p->next) {
        if (p->playlist != playlist)
            continue;
        unknown |= p->start == 0;
        if (p->start == 0)
            continue;
        if (any == 0 || p->start < any)
            any = p->start;
        if (p->origin != PERIOD_AUTO && (sure == 0 || p->start < sure))
            sure = p->start;
    }
    return unknown ? sure : any;
}

/*
** forced by the author, else added in the first days and never removed
*/
static int since_first(library_t *lib, int playlist,
    period_sum_t const *sum, int forced)
{
    long first = 0;

    if (forced != FIRST_AUTO)
        return forced == FIRST_YES;
    if (!sum->in || sum->passes != 1 || sum->added == 0)
        return 0;
    first = first_date(lib, playlist);
    return first > 0 && sum->added <= first + PERIOD_FIRST_S;
}

static void add_period(period_sum_t *sum, period_t const *p, int *forced)
{
    if (p->end == PERIOD_OPEN)
        sum->in = 1;
    if (p->start > sum->added)
        sum->added = p->start;
    if (p->end > sum->removed)
        sum->removed = p->end;
    if (p->first != FIRST_AUTO)
        *forced = p->first;
}

void period_sum(library_t *lib, int playlist, song_t const *song,
    period_sum_t *sum)
{
    int ids[SONG_IDS];
    int nb = song_ids(lib, song, ids);
    int forced = FIRST_AUTO;

    memset(sum, 0, sizeof(period_sum_t));
    for (period_t *p = lib->periods; p; p = p->next) {
        if (p->playlist != playlist || !id_among(p->song, ids, nb))
            continue;
        sum->passes += !counted(lib, p, ids, nb);
        add_period(sum, p, &forced);
    }
    sum->first = since_first(lib, playlist, sum, forced);
}
