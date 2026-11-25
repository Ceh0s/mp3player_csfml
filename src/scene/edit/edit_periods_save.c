/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** stays of the song shown by the page "Découverte": their working copy,
** its check and its saving. The stays may have changed meanwhile (a
** download adds songs to the playlists): each one is found again by what
** it was, and left alone if it is no longer the same.
*/

#include "mp3player.h"

static void date_text(char *buf, long date)
{
    time_t when = date;

    buf[0] = '\0';
    if (date > 0)
        strftime(buf, PERIOD_DATE_LEN, "%Y-%m-%d", localtime(&when));
}

void edit_periods_load(app_t *app)
{
    notes_edit_t *notes = &app->edit->notes;
    period_row_t *row = NULL;
    int ids[SONG_IDS];
    int nb = song_ids(app->lib, app->edit->song, ids);

    for (period_t *p = app->lib->periods; p && notes->nb < PERIOD_ROWS;
        p = p->next) {
        if (!id_among(p->song, ids, nb))
            continue;
        row = &notes->rows[notes->nb];
        row->old = *p;
        row->playlist = p->playlist;
        date_text(row->from, p->start);
        date_text(row->to, p->end);
        notes->nb++;
    }
}

/*
** a date whose text did not change keeps its exact hour; an empty text
** gives none
*/
static long row_date(char const *text, long old, long none)
{
    char before[PERIOD_DATE_LEN];
    long date = 0;

    date_text(before, old);
    if (strcmp(before, text) == 0)
        return old;
    date = stat_date(text, 0);
    return date > 0 ? date : none;
}

/*
** returns 0 (with the reason in msg) if a date is wrong
*/
int edit_periods_check(app_t *app)
{
    notes_edit_t *notes = &app->edit->notes;
    period_row_t *row = NULL;

    notes->msg[0] = '\0';
    for (int i = 0; i < notes->nb && notes->msg[0] == '\0'; i++) {
        row = &notes->rows[i];
        if (row->removed)
            continue;
        if (stat_date(row->from, 0) < 0 || stat_date(row->to, 0) < 0)
            snprintf(notes->msg, sizeof(notes->msg),
                "Date invalide (AAAA-MM-JJ)");
        else if (row->from[0] != '\0' && row->to[0] != '\0'
            && strcmp(row->from, row->to) > 0)
            snprintf(notes->msg, sizeof(notes->msg),
                "Une fin est avant son début");
        if (notes->msg[0] != '\0')
            notes->playlist = row->playlist;
    }
    return notes->msg[0] == '\0';
}

/*
** the stay as it was when the page was opened
*/
static period_t *find_old(library_t *lib, period_t const *old)
{
    for (period_t *p = lib->periods; p; p = p->next)
        if (p->playlist == old->playlist && p->song == old->song
            && p->start == old->start && p->end == old->end)
            return p;
    return NULL;
}

static void add_new(app_t *app, period_row_t const *row)
{
    period_t fresh = {row->playlist, app->edit->song->id, 0, PERIOD_LOST,
        PERIOD_HAND, row->old.first, NULL};

    if (!playlist_by_id(app->lib, row->playlist))
        return;
    fresh.start = row_date(row->from, 0, 0);
    fresh.end = row_date(row->to, 0, PERIOD_LOST);
    period_add(app->lib, &fresh);
}

static void save_row(app_t *app, period_row_t const *row)
{
    period_t *period = NULL;
    long start = row_date(row->from, row->old.start, 0);

    if (row->old.song == 0) {
        if (!row->removed)
            add_new(app, row);
        return;
    }
    period = find_old(app->lib, &row->old);
    if (!period)
        return;
    if (row->removed) {
        period_remove(app->lib, period);
        return;
    }
    if (start != period->start)
        period->origin = PERIOD_HAND;
    period->start = start;
    if (period->end != PERIOD_OPEN)
        period->end = row_date(row->to, row->old.end, PERIOD_LOST);
    period->first = row->old.first;
}

void edit_periods_save(app_t *app)
{
    notes_edit_t const *notes = &app->edit->notes;

    for (int i = 0; i < notes->nb; i++)
        save_row(app, &notes->rows[i]);
    db_save_periods(app->lib);
}
