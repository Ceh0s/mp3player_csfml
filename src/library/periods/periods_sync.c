/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** the stays follow the playlists: called each time the playlists are
** saved, it compares what they hold with the stays going on. A song that
** came in starts a stay, a song that left ends its own (a file removed
** from music/ does not: the song is still in its playlists). The ids are
** resolved once, in an array: this runs often.
** A click made by mistake leaves no trace: a song taken out less than
** PERIOD_UNDO_S after it came in is forgotten, one put back as fast goes
** on with the same stay.
*/

#include "mp3player.h"

/*
** array of lib->next_song ids: the song each id resolves to (an id
** without any song resolves to itself); NULL on error
*/
static int *songs_resolved(library_t *lib)
{
    int *resolved = malloc(sizeof(int) * (lib->next_song + 1));

    if (!resolved)
        return NULL;
    for (int i = 0; i <= lib->next_song; i++)
        resolved[i] = i;
    for (song_t *song = lib->songs; song; song = song->next)
        if (song->merged_into && song->id <= lib->next_song)
            resolved[song->id] = song_resolve(lib, song)->id;
    return resolved;
}

static int valid(library_t const *lib, int id)
{
    return id > 0 && id <= lib->next_song;
}

static void close_period(library_t *lib, period_t *period, long now)
{
    if (period->origin == PERIOD_AUTO
        && now - period->start < PERIOD_UNDO_S) {
        period_remove(lib, period);
        return;
    }
    period->end = now;
}

/*
** id : raw id written in the playlist; the dates of the songs already
** there the first time the history is kept are unknown
*/
static void open_period(library_t *lib, int playlist, int id,
    int const *resolved)
{
    long now = time(NULL);
    period_t fresh = {playlist, id, now, PERIOD_OPEN, PERIOD_AUTO,
        FIRST_AUTO, NULL};

    for (period_t *p = lib->periods; p; p = p->next)
        if (p->playlist == playlist && valid(lib, p->song)
            && resolved[p->song] == resolved[id] && p->end > 0
            && now - p->end < PERIOD_UNDO_S && now >= p->end) {
            p->end = PERIOD_OPEN;
            return;
        }
    if (!lib->settings.periods_started) {
        fresh.start = 0;
        fresh.origin = PERIOD_UNKNOWN;
    }
    period_add(lib, &fresh);
}

/*
** in : 1 for the songs of the playlist, 2 once their stay is found
*/
static void sync_playlist(library_t *lib, playlist_t const *pl,
    int const *resolved, char *in)
{
    long now = time(NULL);
    period_t *next = NULL;

    memset(in, 0, lib->next_song + 1);
    for (int i = 0; i < pl->nb; i++)
        if (valid(lib, pl->songs[i]))
            in[resolved[pl->songs[i]]] = 1;
    for (period_t *p = lib->periods; p; p = next) {
        next = p->next;
        if (p->playlist != pl->id || p->end != PERIOD_OPEN
            || !valid(lib, p->song))
            continue;
        if (in[resolved[p->song]])
            in[resolved[p->song]] = 2;
        else
            close_period(lib, p, now);
    }
    for (int i = 0; i < pl->nb; i++) {
        if (!valid(lib, pl->songs[i]) || in[resolved[pl->songs[i]]] == 2)
            continue;
        open_period(lib, pl->id, pl->songs[i], resolved);
        in[resolved[pl->songs[i]]] = 2;
    }
}

void periods_sync(library_t *lib)
{
    int *resolved = songs_resolved(lib);
    char *in = malloc(lib->next_song + 1);

    for (playlist_t *pl = lib->playlists; resolved && in && pl;
        pl = pl->next)
        sync_playlist(lib, pl, resolved, in);
    if (resolved && in && !lib->settings.periods_started) {
        lib->settings.periods_started = 1;
        db_save_settings(lib);
    }
    free(resolved);
    free(in);
}
