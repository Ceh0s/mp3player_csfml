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
** pixels from the top of the rows to the mouse, scroll included
*/
static float mouse_offset(app_t *app)
{
    return app->mouse.y - ranking_list_top() + (int)app->ranking->list.pos;
}

/*
** insertion place under the mouse: before the row whose upper half
** holds it
*/
static int drop_place(app_t *app)
{
    float pos = (mouse_offset(app) + BOARD_STEP / 2.0) / BOARD_STEP;
    int place = pos < 0 ? 0 : (int)pos;

    return place > app->ranking->nb ? app->ranking->nb : place;
}

void ranking_drag_press(app_t *app)
{
    ranking_t *ranking = app->ranking;
    float pos = (mouse_offset(app) + 8) / BOARD_STEP;

    ranking->drag_row = -1;
    ranking->dragging = 0;
    ranking->press = app->mouse;
    if (!mouse_in_rect(app, ranking->list.area) || pos < 0
        || (int)pos >= ranking->nb || app->mouse.x < 130
        || app->mouse.x > 1780)
        return;
    ranking->drag_row = (int)pos;
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
    float top = ranking_list_top() - (int)ranking->list.pos;
    float rect[4] = {120, top + ranking->drag_row * BOARD_STEP - 6, 1670,
        BOARD_STEP};
    float dx = app->mouse.x - ranking->press.x;
    float dy = app->mouse.y - ranking->press.y;

    if (ranking->drag_row < 0 || dx * dx + dy * dy <= 100
        || !sfMouse_isButtonPressed(sfMouseLeft))
        return;
    widget_rect(app, rect, &held);
    rect[1] = top + drop_place(app) * BOARD_STEP - 8;
    rect[3] = 4;
    widget_rect(app, rect, &line);
}

/*
** a row held above or under the list scrolls it, DRAG_SPEED pixels per
** second
*/
void ranking_drag_scroll(app_t *app)
{
    ranking_t *ranking = app->ranking;
    float pixels = DRAG_SPEED * app->delta_us / 1000000.0;
    float const *area = ranking->list.area;

    if (ranking->tab != TAB_TIES || ranking->drag_row < 0
        || !sfMouse_isButtonPressed(sfMouseLeft))
        return;
    if (app->mouse.y < area[1])
        scroll_by(&ranking->list, -pixels);
    else if (app->mouse.y > area[1] + area[3])
        scroll_by(&ranking->list, pixels);
    else
        return;
    app->tick_loop = TICK_SMOOTH;
}
