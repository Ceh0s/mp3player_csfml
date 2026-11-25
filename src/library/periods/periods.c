/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the stays of the songs in the playlists (period_t), and ids a
** song may have in it (its own and those of the songs merged into it)
*/

#include "mp3player.h"

/*
** a copy of model is added at the end; NULL on error
*/
period_t *period_add(library_t *lib, period_t const *model)
{
    period_t **last = &lib->periods;
    period_t *period = malloc(sizeof(period_t));

    if (!period)
        return NULL;
    *period = *model;
    period->next = NULL;
    while (*last)
        last = &(*last)->next;
    *last = period;
    return period;
}

void period_remove(library_t *lib, period_t *period)
{
    period_t **cur = &lib->periods;

    while (*cur && *cur != period)
        cur = &(*cur)->next;
    if (!*cur)
        return;
    *cur = period->next;
    free(period);
}

/*
** the stays in a playlist (0 = any) of a song (raw id, 0 = any)
*/
void periods_forget(library_t *lib, int playlist, int song)
{
    period_t *next = NULL;

    for (period_t *period = lib->periods; period; period = next) {
        next = period->next;
        if ((playlist == 0 || period->playlist == playlist)
            && (song == 0 || period->song == song))
            period_remove(lib, period);
    }
}

/*
** ids (SONG_IDS of them) : the one of the song first, then those of its
** aliases; returns their number
*/
int song_ids(library_t *lib, song_t const *song, int *ids)
{
    int nb = 1;

    ids[0] = song->id;
    for (song_t *alias = lib->songs; alias && nb < SONG_IDS;
        alias = alias->next) {
        if (alias->merged_into == 0 || alias == song
            || song_resolve(lib, alias) != song)
            continue;
        ids[nb] = alias->id;
        nb++;
    }
    return nb;
}

int id_among(int id, int const *ids, int nb)
{
    for (int i = 0; i < nb; i++)
        if (ids[i] == id)
            return 1;
    return 0;
}

/*
** "12/03/2024" (buf holds 32 bytes)
*/
void period_date(char *buf, long date)
{
    time_t when = date;

    strftime(buf, 32, "%d/%m/%Y", localtime(&when));
}
