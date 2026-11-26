/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** leaderboard of the statistics screen: rank, song (or artist), time
** listened, sessions, skips and Elo if asked, scrolled with the mouse
** wheel; the search only hides rows (the rank stays the real one)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

int stats_rows_shown(app_t *app)
{
    return (CONTENT_BOTTOM + app->extra_h - STATS_Y - BOARD_STEP)
        / BOARD_STEP;
}

static int match(app_t *app, stat_row_t const *row)
{
    if (row->artist)
        return search_text(app, row->artist->name);
    return search_song(app, row->song);
}

/*
** delta = 0 only keeps the scroll valid (after a resize or a change)
*/
void stats_scroll(app_t *app, float delta)
{
    statscene_t *scene = app->stats;
    int nb = 0;

    for (int i = 0; i < scene->nb; i++)
        nb += match(app, &scene->rows[i]);
    if (delta > 0)
        scene->scroll--;
    if (delta < 0)
        scene->scroll++;
    if (scene->scroll > nb - stats_rows_shown(app))
        scene->scroll = nb - stats_rows_shown(app);
    if (scene->scroll < 0)
        scene->scroll = 0;
}

static void header(app_t *app)
{
    widget_text(app, "#", (float [3]){130, STATS_Y, 22}, &gray);
    widget_text(app, app->stats_opt.artists ? "Artiste" : "Musique",
        (float [3]){252, STATS_Y, 22}, &gray);
    widget_text(app, "Temps", (float [3]){1150, STATS_Y, 22}, &gray);
    widget_text(app, "Écoutes", (float [3]){1330, STATS_Y, 22}, &gray);
    widget_text(app, "Skips", (float [3]){1480, STATS_Y, 22}, &gray);
    if (app->stats_opt.elo)
        widget_text(app, "Elo", (float [3]){1630, STATS_Y, 22}, &gray);
}

static void numbers(app_t *app, stat_row_t const *row, float y)
{
    char text[32];

    format_time(text, sizeof(text), row->ms);
    widget_text(app, text, (float [3]){1150, y, 26}, &sfWhite);
    snprintf(text, sizeof(text), "%d", row->plays);
    widget_text(app, text, (float [3]){1330, y, 26}, &sfWhite);
    snprintf(text, sizeof(text), "%d", row->skips);
    widget_text(app, text, (float [3]){1480, y, 26}, &sfWhite);
    snprintf(text, sizeof(text), "%.0f", row->elo);
    if (app->stats_opt.elo)
        widget_text(app, text, (float [3]){1630, y, 26}, &sfWhite);
}

static void draw_line(app_t *app, int i, float y)
{
    stat_row_t const *row = &app->stats->rows[i];
    rank_t rank = {row->song, row->artist, 0, 0, 0};
    char text[16];
    float line[4] = {130, y - 8, 1650, BOARD_STEP - 4};

    if (app->wg->mode == WG_DRAW && mouse_in_rect(app, line))
        app->stats->hover = rank;
    snprintf(text, sizeof(text), "%d", i + 1);
    widget_text(app, text, (float [3]){130, y, 26}, &gray);
    widget_image(app, image_get(app, &rank, 1),
        (float [4]){190, y - 8, THUMB, THUMB}, row->artist != NULL);
    widget_rank(app, &rank, (float [4]){252, y - 8, 870, BOARD_STEP - 4},
        &sfWhite);
    numbers(app, row, y);
}

/*
** shown : rows matching the search before this one
*/
static void lines(app_t *app, statscene_t const *scene)
{
    int shown = -1;
    int rows = stats_rows_shown(app);
    float y = 0;

    for (int i = 0; i < scene->nb; i++) {
        if (!match(app, &scene->rows[i]))
            continue;
        shown++;
        y = STATS_Y + BOARD_STEP + (shown - scene->scroll) * BOARD_STEP;
        if (shown >= scene->scroll && shown < scene->scroll + rows)
            draw_line(app, i, y);
    }
}

void stats_view(app_t *app)
{
    statscene_t *scene = app->stats;
    float empty[3] = {252, STATS_Y + BOARD_STEP, 26};

    stats_bar(app);
    stats_dates(app);
    header(app);
    lines(app, scene);
    if (scene->nb == 0)
        widget_text(app, "Aucun titre ici.", empty, &gray);
    widget_text(app, app->stats_opt.artists ? "Clic droit : page de "
        "l'artiste" : "Clic droit : page de la musique",
        (float [3]){130, 856 + app->extra_h, 22}, &gray);
    if (widget_back(app))
        scene_back(app);
}
