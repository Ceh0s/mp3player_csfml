/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** leaderboard: rank, song, Elo, battles and % of victories,
** scrolled with the mouse wheel; the songs with the same Elo (as it is
** shown) share a rank: the first one shows it, the others "-"
** (1, 2, -, -, 5)
*/

#include "mp3player.h"
#include <math.h>

static const sfColor gray = {180, 180, 180, 255};

/*
** y of the text of the first row of the leaderboard (and of the
** "Départage" tab), under the column names
*/
float ranking_list_top(void)
{
    return CONTENT_Y + BOARD_STEP;
}

/*
** the list of the tab shown goes from top to the bottom of the table
*/
void ranking_list_frame(app_t *app, float top, float content)
{
    float area[4] = {PANEL_X, top, PANEL_W,
        CONTENT_BOTTOM + app->extra_h - top};

    scroll_frame(&app->ranking->list, area, content);
}

/*
** the wheel moves two rows of the tab shown (or of images)
*/
void ranking_scroll(app_t *app, float delta)
{
    ranking_t *ranking = app->ranking;
    float step = BOARD_STEP;

    if (ranking->tab == TAB_HISTORY)
        step = HISTORY_STEP;
    if (ranking->tab == TAB_TIERS)
        step = TILE_MAX + TILE_GAP;
    scroll_by(&ranking->list, -delta * 2 * step);
}

static void header(app_t *app)
{
    widget_text(app, "#", (float [3]){130, CONTENT_Y, 22}, &gray);
    widget_text(app, app->arena.kind == KIND_SONG ? "Musique" : "Artiste",
        (float [3]){252, CONTENT_Y, 22}, &gray);
    widget_text(app, "Elo", (float [3]){1330, CONTENT_Y, 22}, &gray);
    widget_text(app, "Battles", (float [3]){1480, CONTENT_Y, 22}, &gray);
    widget_text(app, "Victoires", (float [3]){1640, CONTENT_Y, 22}, &gray);
}

static void position(app_t *app, int i, float y)
{
    rank_t const *ranks = app->ranking->ranks;
    char text[16];

    if (i > 0 && lround(ranks[i].rating) == lround(ranks[i - 1].rating))
        snprintf(text, sizeof(text), "-");
    else
        snprintf(text, sizeof(text), "%d", i + 1);
    widget_text(app, text, (float [3]){130, y, 26}, &gray);
}

static void draw_line(app_t *app, int i, float y)
{
    rank_t const *rank = &app->ranking->ranks[i];
    char text[32];
    float thumb[4] = {190, y - 8, THUMB, THUMB};

    position(app, i, y);
    ranking_hover(app, rank, (float [4]){130, y - 8, 1650, BOARD_STEP - 4});
    widget_image(app, image_get(app, rank, 1), thumb, rank->artist != NULL);
    widget_rank(app, rank, (float [4]){252, y - 8, 1050, BOARD_STEP - 4},
        &sfWhite);
    snprintf(text, sizeof(text), "%.0f", rank->rating);
    widget_text(app, text, (float [3]){1330, y, 26}, &sfWhite);
    snprintf(text, sizeof(text), "%d", rank->battles);
    widget_text(app, text, (float [3]){1480, y, 26}, &sfWhite);
    snprintf(text, sizeof(text), "%d %%", rank->battles > 0
        ? rank->wins * 100 / rank->battles : 0);
    widget_text(app, text, (float [3]){1640, y, 26}, &sfWhite);
}

/*
** shown : rows matching the search before this one
*/
void ranking_board(app_t *app)
{
    ranking_t *ranking = app->ranking;
    int shown = -1;
    float y = 0;

    header(app);
    ranking_list_frame(app, ranking_list_top() - 8,
        ranking_count(app) * BOARD_STEP);
    if (!scroll_rows_begin(app, &ranking->list))
        return;
    for (int i = 0; i < ranking->nb; i++) {
        if (!ranking_match(app, &ranking->ranks[i]))
            continue;
        shown++;
        y = scroll_y(&ranking->list, shown * BOARD_STEP) + 8;
        if (scroll_seen(&ranking->list, y - 8, BOARD_STEP))
            draw_line(app, i, y);
    }
    scroll_rows_end(app);
}
