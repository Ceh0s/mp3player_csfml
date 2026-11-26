/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** right part of the page "Découverte": the stays of the song in the
** playlist chosen, with their dates, "+ Période" to add one, and the
** mention "depuis le début"
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

int edit_period_shown(notes_edit_t const *notes, int i)
{
    return notes->rows[i].playlist == notes->playlist
        && !notes->rows[i].removed;
}

static void date_field(app_t *app, edit_t *edit, float const *rect,
    int const *where)
{
    notes_edit_t *notes = &edit->notes;
    period_row_t *row = &notes->rows[where[0]];
    int focused = edit->focus == FOCUS_PERIOD && notes->row == where[0]
        && notes->side == where[1];

    widget_help(app, rect, "AAAA-MM-JJ");
    if (!widget_field(app, rect, where[1] ? row->to : row->from, focused))
        return;
    edit->focus = FOCUS_PERIOD;
    notes->row = where[0];
    notes->side = where[1];
}

/*
** the stay going on ends when the song is taken out of the playlist, it
** is not removed here; returns 1 if the row was removed
*/
static int row_view(app_t *app, edit_t *edit, int i, float y)
{
    period_row_t *row = &edit->notes.rows[i];
    int open = row->old.song != 0 && row->old.end == PERIOD_OPEN;

    widget_text(app, "du", (float [3]){1090, y + 10, 24}, &gray);
    date_field(app, edit, (float [4]){1135, y, 200, 50}, (int [2]){i, 0});
    if (open) {
        widget_text(app, "en cours", (float [3]){1355, y + 10, 24},
            &sfWhite);
        return 0;
    }
    widget_text(app, "au", (float [3]){1355, y + 10, 24}, &gray);
    date_field(app, edit, (float [4]){1400, y, 200, 50}, (int [2]){i, 1});
    if (!widget_button(app, (float [4]){1620, y, 160, 50}, "Retirer"))
        return 0;
    row->removed = 1;
    edit->notes.msg[0] = '\0';
    edit->focus = FOCUS_FOUND;
    return 1;
}

/*
** the new stay is at the end: the list goes down to it
*/
static void add_row(edit_t *edit)
{
    notes_edit_t *notes = &edit->notes;
    period_row_t *row = NULL;

    if (notes->nb >= PERIOD_ROWS)
        return;
    row = &notes->rows[notes->nb];
    memset(row, 0, sizeof(period_row_t));
    row->playlist = notes->playlist;
    row->old.end = PERIOD_LOST;
    edit->focus = FOCUS_PERIOD;
    notes->row = notes->nb;
    notes->side = 0;
    notes->nb++;
    for (int i = 0; i < notes->nb; i++)
        if (edit_period_shown(notes, i))
            row->old.first = notes->rows[i].old.first;
    scroll_jump(&notes->periods, notes->periods.content + 58);
}

/*
** the choice is kept on every stay of the playlist
*/
static void first_button(app_t *app, notes_edit_t *notes, float y)
{
    char const *const labels[NB_FIRSTS] = {"Depuis le début : auto",
        "Depuis le début : oui", "Depuis le début : non"};
    float rect[4] = {1330, y, 450, 50};
    int first = -1;

    for (int i = 0; i < notes->nb && first < 0; i++)
        if (edit_period_shown(notes, i))
            first = notes->rows[i].old.first;
    if (first < 0)
        return;
    widget_help(app, rect, "Auto : ajoutée dans les 7 premiers jours, "
        "jamais retirée");
    if (!widget_button(app, rect, labels[first]))
        return;
    for (int i = 0; i < notes->nb; i++)
        if (edit_period_shown(notes, i))
            notes->rows[i].old.first = (first + 1) % NB_FIRSTS;
}

void edit_periods(app_t *app)
{
    edit_t *edit = app->edit;
    notes_edit_t *notes = &edit->notes;
    float bottom = 770 + app->extra_h;
    float area[4] = {1080, 356, PANEL_X + PANEL_W - 1080, bottom - 8 - 356};
    int row = 0;
    float y = 0;

    if (notes->playlist == 0)
        return;
    widget_text(app, "Périodes", (float [3]){1090, 315, 24}, &sfWhite);
    for (int i = 0; i < notes->nb; i++)
        row += edit_period_shown(notes, i);
    scroll_frame(&notes->periods, area, row * 58);
    scroll_bar_draw(app, &notes->periods);
    row = 0;
    if (scroll_rows_begin(app, &notes->periods)) {
        for (int i = 0; i < notes->nb; i++) {
            if (!edit_period_shown(notes, i))
                continue;
            y = scroll_y(&notes->periods, row * 58 + 4);
            row++;
            if (scroll_seen(&notes->periods, y, 50)
                && row_view(app, edit, i, y))
                break;
        }
        scroll_rows_end(app);
    }
    if (widget_button(app, (float [4]){1090, bottom, 220, 50}, "+ Période"))
        add_row(edit);
    first_button(app, notes, bottom);
}
