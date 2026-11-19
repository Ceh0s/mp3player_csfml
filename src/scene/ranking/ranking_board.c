/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** leaderboard: rank, song, Elo, battles and % of victories,
** scrolled with the mouse wheel
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static int visible_rows(app_t *app)
{
    return (CONTENT_BOTTOM + app->extra_h - CONTENT_Y - BOARD_STEP)
        / BOARD_STEP;
}

/*
** delta = 0 only keeps the scroll valid (after a resize)
*/
void ranking_scroll(app_t *app, float delta)
{
    ranking_t *ranking = app->ranking;
    int max = ranking->nb - visible_rows(app);

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
    widget_text(app, "Musique", (float [3]){210, CONTENT_Y, 22}, &gray);
    widget_text(app, "Elo", (float [3]){1330, CONTENT_Y, 22}, &gray);
    widget_text(app, "Battles", (float [3]){1480, CONTENT_Y, 22}, &gray);
    widget_text(app, "Victoires", (float [3]){1640, CONTENT_Y, 22}, &gray);
}

static void draw_line(app_t *app, rank_t const *rank, int position,
    float y)
{
    char text[NAME_LEN * 2];
    song_t const *song = rank->song;

    snprintf(text, sizeof(text), "%d", position + 1);
    widget_text(app, text, (float [3]){130, y, 26}, &gray);
    snprintf(text, sizeof(text), "%s%s%s", song_title(song),
        song->artists ? " - " : "", song->artists ? song->artists : "");
    widget_text(app, text, (float [3]){210, y, 26}, &sfWhite);
    snprintf(text, sizeof(text), "%.0f", rank->rating);
    widget_text(app, text, (float [3]){1330, y, 26}, &sfWhite);
    snprintf(text, sizeof(text), "%d", rank->battles);
    widget_text(app, text, (float [3]){1480, y, 26}, &sfWhite);
    snprintf(text, sizeof(text), "%d %%", rank->battles > 0
        ? rank->wins * 100 / rank->battles : 0);
    widget_text(app, text, (float [3]){1640, y, 26}, &sfWhite);
}

void ranking_board(app_t *app)
{
    ranking_t *ranking = app->ranking;
    int rows = visible_rows(app);
    float y = 0;

    header(app);
    for (int i = ranking->scroll; i < ranking->nb
        && i < ranking->scroll + rows; i++) {
        y = CONTENT_Y + BOARD_STEP + (i - ranking->scroll) * BOARD_STEP;
        draw_line(app, &ranking->ranks[i], i, y);
    }
}
