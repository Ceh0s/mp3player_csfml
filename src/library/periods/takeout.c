/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** import of the dates a playlist of YouTube Music keeps (Google Takeout):
** only the current stay of each song is known by the site. An unknown
** date is replaced, a date seen by the app or imported before only by an
** older one (not from a stay that ended); a date typed by the author
** never is. A date imported before and now later by more than a day means
** the song was taken out of the playlist of the site, then put back: a
** new stay starts.
*/

#include "mp3player.h"

typedef struct found_s {
    song_t *song;
    long date;
} found_t;

/*
** a song written twice (two videos of it) keeps its oldest date; returns
** the new number of songs
*/
static int add_found(found_t *found, int nb, song_t *song, long date)
{
    for (int i = 0; i < nb; i++) {
        if (found[i].song != song)
            continue;
        if (date < found[i].date)
            found[i].date = date;
        return nb;
    }
    if (nb >= TAKEOUT_MAX)
        return nb;
    found[nb].song = song;
    found[nb].date = date;
    return nb + 1;
}

static int read_file(library_t *lib, playlist_t *playlist, FILE *file,
    found_t *found, takeout_t *out)
{
    char id[VIDEO_ID];
    char *line = NULL;
    size_t size = 0;
    long date = 0;
    song_t *song = NULL;
    int nb = 0;

    while (getline(&line, &size, file) > 0) {
        if (!takeout_line(line, id, &date))
            continue;
        out->lines++;
        song = takeout_song(lib, id);
        if (!song || !playlist_has_song(lib, playlist, song))
            continue;
        out->in_list++;
        nb = add_found(found, nb, song, date);
    }
    free(line);
    return nb;
}

/*
** the song was put back in the playlist of the site
*/
static void new_stay(library_t *lib, period_t *period, long date)
{
    period_t fresh = *period;

    fresh.start = date;
    fresh.first = FIRST_AUTO;
    if (period_add(lib, &fresh))
        period->end = PERIOD_LOST;
}

/*
** 1 if the date belongs to a stay that ended (a file exported before the
** song was put back): it says nothing of the stay going on
*/
static int old_stay(library_t *lib, int playlist, found_t const *found)
{
    int ids[SONG_IDS];
    int nb = song_ids(lib, found->song, ids);

    for (period_t *p = lib->periods; p; p = p->next)
        if (p->playlist == playlist && p->end != PERIOD_OPEN
            && id_among(p->song, ids, nb) && found->date
            <= (p->end == PERIOD_LOST ? p->start : p->end))
            return 1;
    return 0;
}

/*
** returns 1 if something changed
*/
static int apply(library_t *lib, playlist_t const *playlist,
    found_t const *found)
{
    period_t *period = period_current(lib, playlist->id, found->song);

    if (!period || period->origin == PERIOD_HAND
        || period->start == found->date
        || old_stay(lib, playlist->id, found))
        return 0;
    if (period->start == 0 || found->date < period->start) {
        period->start = found->date;
        period->origin = PERIOD_IMPORT;
        return 1;
    }
    if (period->origin != PERIOD_IMPORT
        || found->date <= period->start + PERIOD_DAY)
        return 0;
    new_stay(lib, period, found->date);
    return 1;
}

/*
** returns 84 if the file cannot be read; force = 1 imports it even if
** most of its videos are not in the playlist (out->few)
*/
int takeout_import(library_t *lib, playlist_t *playlist, char const *path,
    int force, takeout_t *out)
{
    FILE *file = fopen(path, "r");
    found_t *found = file ? malloc(sizeof(found_t) * TAKEOUT_MAX) : NULL;
    int nb = 0;

    memset(out, 0, sizeof(takeout_t));
    if (!found) {
        if (file)
            fclose(file);
        return 84;
    }
    periods_sync(lib);
    nb = read_file(lib, playlist, file, found, out);
    fclose(file);
    out->few = out->in_list * 2 < out->lines && !force;
    for (int i = 0; i < nb && !out->few; i++)
        out->changed += apply(lib, playlist, &found[i]);
    free(found);
    db_save_periods(lib);
    return 0;
}
