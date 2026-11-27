/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** stats of a song or an artist in a window over its page: one bar from
** 0 to 100 per stat shown (its rank in the ranking chosen in the battle
** and rankings screens, app->arena.board, elo_percent),
** red to green (the other way for a flaw); "Choisir les stats" shows them
** all with a box, choice saved for every page (categories.tsv). The stats
** set by hand (songs only) are set by a click on their bar.
*/

#include "mp3player.h"

static const sfColor gray = {190, 190, 190, 255};

    #define PANEL_ROW 46
    #define BAR_X 790
    #define BAR_W 540

static float row_y(int row)
{
    return 240 + row * PANEL_ROW;
}

/*
** values are read once, when the window opens
*/
void stat_panel_open(app_t *app, statpanel_t *panel, rank_t const *subject)
{
    library_t *lib = app->lib;
    arena_t arena = {subject->song ? KIND_SONG : KIND_ARTIST,
        app->arena.board, 0};

    panel->open = 1;
    panel->subject = *subject;
    if (subject->song)
        panel->subject.song = song_resolve(lib, subject->song);
    for (int i = 0; i < NB_STATS; i++) {
        arena.category = i;
        if (!stat_exists(lib, i))
            panel->values[i] = -2;
        else if (i < MAX_CATEGORIES)
            panel->values[i] = elo_percent(lib, &arena,
                rank_id(&panel->subject));
        else if (panel->subject.song)
            panel->values[i] = manual_get(lib, panel->subject.song, i);
        else
            panel->values[i] = -2;
    }
}

/*
** red -> yellow -> green as good goes from 0 to 1
*/
static sfColor bar_color(float good)
{
    float up = good * 2 - 1;

    if (good < 0.5)
        return (sfColor){220 + 20 * good, 60 + 280 * good, 60, 255};
    return (sfColor){230 - 160 * up, 200 - 10 * up, 60 + 30 * up, 255};
}

static void draw_bar(app_t *app, int stat, int value, float y)
{
    float track[4] = {BAR_X, y + 14, BAR_W, 18};
    sfColor back = {255, 255, 255, 40};
    sfColor fill = {0};
    float good = value / 100.0;
    char text[8];

    widget_rect(app, track, &back);
    snprintf(text, sizeof(text), "-");
    if (value >= 0) {
        if (app->lib->cats[stat].flaw)
            good = 1 - good;
        fill = bar_color(good);
        track[2] = BAR_W * value / 100.0;
        widget_rect(app, track, &fill);
        snprintf(text, sizeof(text), "%d", value);
    }
    widget_text(app, text, (float [3]){BAR_X + BAR_W + 24, y + 6, 26},
        &sfWhite);
}

/*
** a stat set by hand: a click on its bar sets it (steps of 5), the
** cross clears it
*/
static void manual_row(app_t *app, statpanel_t *panel, int stat, float y)
{
    float track[4] = {BAR_X, y + 4, BAR_W, 38};
    float clear[4] = {BAR_X + BAR_W + 90, y + 2, 40, 40};
    int value = 0;

    if (panel->values[stat] >= 0 && widget_icon_tip(app, clear, ICON_CLOSE,
            "Effacer"))
        value = -1;
    else if (app->wg->mode == WG_CLICK && mouse_in_rect(app, track)) {
        value = (int)((app->mouse.x - BAR_X) * 20 / BAR_W + 0.5) * 5;
        app->wg->mode = WG_IDLE;
    } else
        return;
    manual_set(app->lib, panel->subject.song->id, stat, value);
    db_save_manual(app->lib);
    panel->values[stat] = value;
}

static void stat_row(app_t *app, statpanel_t *panel, int stat, float y)
{
    cat_opt_t *opt = &app->lib->cats[stat];
    float name[4] = {450, y, 320, PANEL_ROW - 4};
    float box[4] = {396, y + 2, 38, 38};

    if (panel->choose && widget_icon(app, box, ICON_CHECK,
            opt->shown ? ICON_FILLED : ICON_DIM)) {
        opt->shown = !opt->shown;
        db_save_categories(app->lib);
    }
    widget_tip(app, name, opt->desc);
    widget_fit(app, stat_name(app->lib, stat), (float [4]){450, y + 6, 24, 320},
        &sfWhite);
    draw_bar(app, stat, panel->values[stat], y);
    if (stat >= MAX_CATEGORIES)
        manual_row(app, panel, stat, y);
}

/*
** the stats shown (every one while choosing), those of the songs only
** left out for an artist
*/
static void rows(app_t *app, statpanel_t *panel)
{
    panel->nb = 0;
    for (int stat = 0; stat < NB_STATS; stat++) {
        if (panel->values[stat] == -2
            || (!panel->choose && !app->lib->cats[stat].shown))
            continue;
        stat_row(app, panel, stat, row_y(panel->nb));
        panel->rows[panel->nb] = stat;
        panel->nb++;
    }
    if (panel->nb == 0)
        widget_text(app, "Aucune stat choisie.",
            (float [3]){450, row_y(0) + 6, 24}, &gray);
}

static void header(app_t *app, statpanel_t *panel)
{
    float choose[4] = {1440, 170, 48, 48};

    board_t *board = board_by_id(app->lib, app->arena.board);
    char line[NAME_LEN * 2 + 8];

    snprintf(line, sizeof(line), "%s    |    %s", rank_name(&panel->subject),
        board ? board->name : "?");
    widget_text(app, "Stats", (float [3]){400, 165, 36}, &sfWhite);
    widget_fit(app, line, (float [4]){540, 178, 24, 860}, &gray);
    widget_tip(app, choose, "Choisir les stats");
    if (widget_icon(app, choose, ICON_SLIDERS, panel->choose ? ICON_FILLED
            : ICON_OUTLINE))
        panel->choose = !panel->choose;
    if (widget_icon_tip(app, (float [4]){1572, 150, 48, 48}, ICON_CLOSE,
            "Fermer"))
        panel->open = 0;
}

/*
** the window takes every click while it is open; called first when
** clicking, last when drawing
*/
void stat_panel(app_t *app, statpanel_t *panel)
{
    sfFloatRect view = app->view_rect;
    sfColor shade = {0, 0, 0, 170};
    sfColor back = {30, 30, 30, 245};
    char const *help = panel->subject.song
        ? "Clic sur la barre de « Biais de récence » : régler    |    "
        "Clic droit : description" : "Clic droit : description";

    if (!panel->open)
        return;
    widget_rect(app, (float [4]){view.left, view.top, view.width,
        view.height}, &shade);
    widget_rect(app, (float [4]){360, 150, 1200, 790}, &back);
    header(app, panel);
    rows(app, panel);
    widget_text(app, help, (float [3]){400, 900, 20}, &gray);
    if (app->wg->mode == WG_CLICK)
        app->wg->mode = WG_IDLE;
}

/*
** a right click on a row opens the window of its description
*/
void stat_panel_right(app_t *app, statpanel_t *panel)
{
    float rect[4] = {390, 0, 1150, PANEL_ROW - 4};

    for (int row = 0; row < panel->nb; row++) {
        rect[1] = row_y(row);
        if (mouse_in_rect(app, rect))
            catdesc_open(app, panel->rows[row]);
    }
}
