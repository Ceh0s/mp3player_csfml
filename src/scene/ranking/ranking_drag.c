/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** drag and drop in the "Départage" tab: a row pressed, moved by more
** than 10 pixels and released where it goes changes the order chosen
** between ex aequo (saved at once)
*/

#include "mp3player.h"

/*
** first row of the list, under the header line
*/
static float list_top(void)
{
    return CONTENT_Y + BOARD_STEP;
}

/*
** insertion place under the mouse: before the row whose upper half
** holds it, kept inside the rows shown
*/
static int drop_place(app_t *app)
{
    ranking_t *ranking = app->ranking;
    float pos = (app->mouse.y - list_top() + BOARD_STEP / 2.0) / BOARD_STEP;
    int place = ranking->scroll + (pos < 0 ? 0 : (int)pos);
    int last = ranking->scroll + ranking_rows(app);

    if (last > ranking->nb)
        last = ranking->nb;
    return place > last ? last : place;
}

void ranking_drag_press(app_t *app)
{
    ranking_t *ranking = app->ranking;
    float pos = (app->mouse.y - list_top()) / BOARD_STEP;
    int row = ranking->scroll + (int)pos;

    ranking->drag_row = -1;
    ranking->dragging = 0;
    ranking->press = app->mouse;
    if (pos < 0 || pos >= ranking_rows(app) || row >= ranking->nb
        || app->mouse.x < 130 || app->mouse.x > 1780)
        return;
    ranking->drag_row = row;
}

static void move_row(app_t *app, int from, int to)
{
    rank_t *order = app->ranking->order;
    rank_t moved = order[from];

    if (to > from)
        to--;
    if (to == from)
        return;
    if (to > from)
        memmove(&order[from], &order[from + 1], sizeof(rank_t) * (to - from));
    else
        memmove(&order[to + 1], &order[to], sizeof(rank_t) * (from - to));
    order[to] = moved;
    ranking_save_order(app);
}

/*
** returns 1 if a row was dragged (the release is then not a click)
*/
int ranking_drag_release(app_t *app)
{
    ranking_t *ranking = app->ranking;
    int from = ranking->drag_row;
    float dx = app->mouse.x - ranking->press.x;
    float dy = app->mouse.y - ranking->press.y;

    ranking->drag_row = -1;
    if (from < 0 || from >= ranking->nb || dx * dx + dy * dy <= 100)
        return 0;
    move_row(app, from, drop_place(app));
    return 1;
}

/*
** the row held is lit, a line shows where it will be inserted
*/
void ranking_drag_draw(app_t *app)
{
    ranking_t *ranking = app->ranking;
    sfColor held = {255, 200, 0, 60};
    sfColor line = {255, 200, 0, 255};
    int row = ranking->drag_row - ranking->scroll;
    float rect[4] = {120, list_top() + row * BOARD_STEP - 6, 1670,
        BOARD_STEP};
    float dx = app->mouse.x - ranking->press.x;
    float dy = app->mouse.y - ranking->press.y;

    if (ranking->drag_row < 0 || dx * dx + dy * dy <= 100
        || !sfMouse_isButtonPressed(sfMouseLeft))
        return;
    if (row >= 0 && row < ranking_rows(app))
        widget_rect(app, rect, &held);
    rect[1] = list_top() + (drop_place(app) - ranking->scroll) * BOARD_STEP
        - 8;
    rect[3] = 4;
    widget_rect(app, rect, &line);
}
