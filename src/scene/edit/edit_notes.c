/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page "Découverte" of a song: where and when it was found (free texts),
** the playlists with the number of stays of the song in each one, and the
** dates of these stays for the playlist chosen (edit_periods.c). It is a
** working copy, saved with "Enregistrer".
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor orange = {255, 160, 60, 255};

void edit_notes_open(app_t *app)
{
    edit_t *edit = app->edit;
    note_t *note = note_find(app->lib, edit->song);
    notes_edit_t *notes = &edit->notes;

    memset(notes, 0, sizeof(notes_edit_t));
    snprintf(notes->found, NAME_LEN, "%s", note ? note->found : "");
    snprintf(notes->date, NAME_LEN, "%s", note ? note->date : "");
    for (playlist_t *pl = app->lib->playlists; pl; pl = pl->next)
        if (notes->playlist == 0 || (pl->auto_like == 0
                && playlist_by_id(app->lib, notes->playlist)->auto_like))
            notes->playlist = pl->id;
    edit_periods_load(app);
    edit->mode = EDIT_NOTES;
    edit->focus = FOCUS_FOUND;
}

static void fields(app_t *app, edit_t *edit)
{
    notes_edit_t *notes = &edit->notes;

    widget_text(app, "Découverte", (float [3]){130, 125, 40}, &sfWhite);
    widget_fit(app, song_title(edit->song), (float [4]){400, 138, 26, 1380},
        &gray);
    widget_text(app, "Trouvée où", (float [3]){130, 195, 24}, &sfWhite);
    if (widget_field(app, (float [4]){130, 230, 1080, 54}, notes->found,
            edit->focus == FOCUS_FOUND))
        edit->focus = FOCUS_FOUND;
    widget_text(app, "Date de découverte",
        (float [3]){1240, 195, 24}, &sfWhite);
    if (widget_field(app, (float [4]){1240, 230, 540, 54}, notes->date,
            edit->focus == FOCUS_FOUND_DATE))
        edit->focus = FOCUS_FOUND_DATE;
}

/*
** "jamais", "1 passage, depuis le 12/03/2024", "3 passages, retirée le
** ..." (buf holds 128 bytes)
*/
static void status(library_t *lib, int playlist, song_t const *song,
    char *buf)
{
    period_sum_t sum;
    char date[32];
    char const *link = "";

    period_sum(lib, playlist, song, &sum);
    if (sum.passes == 0) {
        snprintf(buf, 128, "jamais");
        return;
    }
    strcpy(date, "date inconnue");
    if (sum.in && sum.added > 0) {
        period_date(date, sum.added);
        link = "depuis le ";
    } else if (!sum.in && sum.removed > 0) {
        period_date(date, sum.removed);
        link = "retirée le ";
    } else if (!sum.in)
        link = "retirée, ";
    snprintf(buf, 128, "%d passage%s, %s%s%s", sum.passes,
        sum.passes > 1 ? "s" : "", link, date,
        sum.first ? " (depuis le début)" : "");
}

/*
** the playlists, left column, from y 356 to the buttons; they scroll
*/
static void frame(app_t *app, notes_edit_t *notes)
{
    float area[4] = {120, 356, 930, 820 + app->extra_h - 356};
    int nb = 0;

    for (playlist_t *pl = app->lib->playlists; pl; pl = pl->next)
        nb++;
    scroll_frame(&notes->lists, area, nb * 56);
}

static void playlists(app_t *app, edit_t *edit)
{
    notes_edit_t *notes = &edit->notes;
    int row = 0;
    float y = 0;
    char text[NAME_LEN + 8];

    widget_text(app, "Passages dans les playlists", (float [3]){130, 315, 24},
        &sfWhite);
    frame(app, notes);
    scroll_bar_draw(app, &notes->lists);
    if (!scroll_rows_begin(app, &notes->lists))
        return;
    for (playlist_t *pl = app->lib->playlists; pl; pl = pl->next) {
        y = scroll_y(&notes->lists, row * 56 + 4);
        row++;
        if (!scroll_seen(&notes->lists, y, 48))
            continue;
        snprintf(text, sizeof(text), pl->id == notes->playlist ? "> %s <"
            : "%s", pl->name);
        if (widget_button(app, (float [4]){130, y, 380, 48}, text)) {
            notes->playlist = pl->id;
            scroll_jump(&notes->periods, 0);
            edit->focus = FOCUS_FOUND;
        }
        status(app->lib, pl->id, edit->song, text);
        widget_fit(app, text, (float [4]){530, y + 10, 22, 530}, &gray);
    }
    scroll_rows_end(app);
}

/*
** the wheel moves the list under the mouse, two rows
*/
void edit_notes_scroll(app_t *app, float delta)
{
    notes_edit_t *notes = &app->edit->notes;

    if (app->mouse.x < 1080)
        scroll_by(&notes->lists, -delta * 2 * 56);
    else
        scroll_by(&notes->periods, -delta * 2 * 58);
}

/*
** each column has its slider
*/
void edit_notes_update(app_t *app)
{
    notes_edit_t *notes = &app->edit->notes;

    scroll_grab(app, &notes->lists);
    scroll_grab(app, &notes->periods);
    scroll_update(app, &notes->lists);
    scroll_update(app, &notes->periods);
}

static void buttons(app_t *app, edit_t *edit)
{
    notes_edit_t *notes = &edit->notes;
    float y = 840 + app->extra_h;
    int save = widget_button(app, (float [4]){130, y, 260, 56},
        "Enregistrer");
    int back = widget_button(app, (float [4]){410, y, 260, 56}, "Annuler");

    widget_text(app, notes->msg, (float [3]){700, y + 14, 24}, &orange);
    if (save && edit_periods_check(app)) {
        note_set(app->lib, edit->song, notes->found, notes->date);
        edit_periods_save(app);
        back = 1;
    }
    if (!back)
        return;
    edit->mode = EDIT_MAIN;
    edit->focus = FOCUS_TITLE;
}

void edit_notes(app_t *app)
{
    edit_t *edit = app->edit;

    fields(app, edit);
    playlists(app, edit);
    edit_periods(app);
    buttons(app, edit);
}
