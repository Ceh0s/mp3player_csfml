/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** under the artists of the edit screen: where and when the song was found
** (if typed, page "Découverte") and the bars of the stats shown (same
** choice as the window "Stats"); the block scrolls when it lacks room
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

    #define SUMMARY_ROW 36

/*
** the Elo are read when the page opens (and after a split); the stats set
** by hand are read again at each drawing
*/
void edit_summary_read(app_t *app)
{
    edit_t *edit = app->edit;
    rank_t rank = {song_resolve(app->lib, edit->song), NULL, 0, 0, 0};

    stat_values_read(app, &rank, edit->stat_values);
}

static int stat_value(app_t *app, edit_t *edit, int stat)
{
    if (stat < MAX_CATEGORIES || edit->stat_values[stat] == -2)
        return edit->stat_values[stat];
    return manual_get(app->lib, song_resolve(app->lib, edit->song), stat);
}

static int stat_shown(app_t *app, edit_t *edit, int stat)
{
    return edit->stat_values[stat] != -2 && app->lib->cats[stat].shown;
}

static int nb_rows(app_t *app, edit_t *edit, note_t const *note)
{
    int nb = 0;

    if (note && note->found[0])
        nb++;
    if (note && note->date[0])
        nb++;
    for (int stat = 0; stat < NB_STATS; stat++)
        nb += stat_shown(app, edit, stat);
    return nb;
}

static void note_line(app_t *app, char const *label, char const *text,
    float y)
{
    char line[NAME_LEN + 32];

    snprintf(line, sizeof(line), "%s : %s", label, text);
    widget_fit(app, line, (float [4]){130, y + 6, 22, 1030}, &gray);
}

static void stat_line(app_t *app, edit_t *edit, int stat, float y)
{
    widget_tip(app, (float [4]){130, y, 300, SUMMARY_ROW - 4},
        app->lib->cats[stat].desc);
    widget_fit(app, stat_name(app->lib, stat),
        (float [4]){130, y + 6, 22, 300}, &sfWhite);
    stat_bar(app, stat, stat_value(app, edit, stat),
        (float [4]){450, y + 11, 560, 14});
}

/*
** rows in order: the two notes, then the stats; row = index of the row
** drawn in the content
*/
static void rows(app_t *app, edit_t *edit, note_t const *note)
{
    scroll_t *s = &edit->summary;
    int row = 0;
    int stat = 0;
    float y = 0;

    if (note && note->found[0]) {
        y = scroll_y(s, row++ * SUMMARY_ROW);
        if (scroll_seen(s, y, SUMMARY_ROW))
            note_line(app, "Trouvée où", note->found, y);
    }
    if (note && note->date[0]) {
        y = scroll_y(s, row++ * SUMMARY_ROW);
        if (scroll_seen(s, y, SUMMARY_ROW))
            note_line(app, "Découverte", note->date, y);
    }
    for (int i = 0; i < NB_STATS; i++) {
        stat = stat_at(app->lib, i);
        if (stat < 0 || !stat_shown(app, edit, stat))
            continue;
        y = scroll_y(s, row++ * SUMMARY_ROW);
        if (scroll_seen(s, y, SUMMARY_ROW))
            stat_line(app, edit, stat, y);
    }
}

/*
** left column, from under the last artist to the buttons
*/
void edit_summary(app_t *app, edit_t *edit)
{
    note_t *note = note_find(app->lib, edit->song);
    float top = 440 + edit->nb_artists * 54 + 20;
    float area[4] = {130, top, 1030, 825 + app->extra_h - top};
    int nb = nb_rows(app, edit, note);

    if (area[3] < SUMMARY_ROW)
        area[3] = 0;
    scroll_frame(&edit->summary, area, nb * SUMMARY_ROW);
    if (nb == 0 || area[3] == 0)
        return;
    scroll_bar_draw(app, &edit->summary);
    if (!scroll_rows_begin(app, &edit->summary))
        return;
    rows(app, edit, note);
    scroll_rows_end(app);
}
