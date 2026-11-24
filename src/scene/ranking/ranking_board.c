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
** rows of the leaderboard (and of the "Départage" tab) on the screen
*/
int ranking_rows(app_t *app)
{
    if (app->ranking->tab == TAB_HISTORY)
        return ranking_history_rows(app);
    return (CONTENT_BOTTOM + app->extra_h - CONTENT_Y - BOARD_STEP)
        / BOARD_STEP;
}

/*
** delta = 0 only keeps the scroll valid (after a resize)
*/
void ranking_scroll(app_t *app, float delta)
{
    ranking_t *ranking = app->ranking;
    int max = ranking_count(app) - ranking_rows(app);

    if (delta > 0)
        ranking->scroll--;
    if (delta < 0)
        ranking->scroll++;
    if (ranking->scroll > max)
        ranking->scroll = max;
    if (ranking->scroll < 0)
        ranking->scroll = 0;
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
    int rows = ranking_rows(app);
    int shown = -1;

    header(app);
    for (int i = 0; i < ranking->nb; i++) {
        if (!ranking_match(app, &ranking->ranks[i]))
            continue;
        shown++;
        if (shown < ranking->scroll || shown >= ranking->scroll + rows)
            continue;
        draw_line(app, i, CONTENT_Y + BOARD_STEP
            + (shown - ranking->scroll) * BOARD_STEP);
    }
}
