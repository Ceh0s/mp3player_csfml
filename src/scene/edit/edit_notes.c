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
    widget_text(app, "Trouvée où (Shazam, suggestion, restaurant...)",
        (float [3]){130, 195, 24}, &sfWhite);
    if (widget_field(app, (float [4]){130, 230, 1080, 54}, notes->found,
            edit->focus == FOCUS_FOUND))
        edit->focus = FOCUS_FOUND;
    widget_text(app, "Date de découverte (texte libre)",
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

static int list_rows(app_t *app)
{
    return (820 + app->extra_h - 360) / 56;
}

static void playlists(app_t *app, edit_t *edit)
{
    notes_edit_t *notes = &edit->notes;
    int row = -notes->scroll;
    float y = 0;
    char text[NAME_LEN + 8];

    widget_text(app, "Passages dans les playlists", (float [3]){130, 315, 24},
        &sfWhite);
    for (playlist_t *pl = app->lib->playlists; pl; pl = pl->next) {
        y = 360 + row * 56;
        row++;
        if (row <= 0 || row > list_rows(app))
            continue;
        snprintf(text, sizeof(text), pl->id == notes->playlist ? "> %s <"
            : "%s", pl->name);
        if (widget_button(app, (float [4]){130, y, 380, 48}, text)) {
            notes->playlist = pl->id;
            notes->scroll_rows = 0;
            edit->focus = FOCUS_FOUND;
        }
        status(app->lib, pl->id, edit->song, text);
        widget_fit(app, text, (float [4]){530, y + 10, 22, 530}, &gray);
    }
}

/*
** the wheel moves the list under the mouse
*/
void edit_notes_scroll(app_t *app, float delta)
{
    notes_edit_t *notes = &app->edit->notes;
    int *scroll = app->mouse.x < 1080 ? &notes->scroll
        : &notes->scroll_rows;
    int nb = 0;

    if (scroll == &notes->scroll)
        for (playlist_t *pl = app->lib->playlists; pl; pl = pl->next)
            nb++;
    for (int i = 0; scroll != &notes->scroll && i < notes->nb; i++)
        nb += edit_period_shown(notes, i);
    *scroll -= (int)delta;
    if (*scroll > nb - 1)
        *scroll = nb - 1;
    if (*scroll < 0)
        *scroll = 0;
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
