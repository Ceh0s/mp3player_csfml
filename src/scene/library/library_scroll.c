/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scroll of the list of the library, in pixels: the wheel and the drag
** move the place aimed at (scroll_to), the one shown (scroll) slides to
** it. A slider at the right of the list shows the place and moves it;
** both are drawn faster than the rest (TICK_SMOOTH) while they move.
*/

#include "mp3player.h"

#include <math.h>

static float scroll_max(libscene_t const *scene)
{
    int hidden = scene->nb_view * LIST_STEP - scene->height;

    return hidden > 0 ? hidden : 0;
}

static float bounded(float value, float max)
{
    if (value > max)
        value = max;
    return value < 0 ? 0 : value;
}

/*
** pixels > 0 goes down the list; 0 only keeps the scroll valid (after a
** resize or a change of the list)
*/
void list_scroll(app_t *app, float pixels)
{
    libscene_t *scene = app->libscene;
    float max = scroll_max(scene);

    scene->scroll_to = bounded(scene->scroll_to + pixels, max);
    scene->scroll = bounded(scene->scroll, max);
}

/*
** y of a row of the list, as it is shown now (it may be partly, or
** fully, out of the list)
*/
float list_row_y(libscene_t const *scene, int row)
{
    return LIST_Y + row * LIST_STEP - (int)scene->scroll;
}

/*
** the rows shown are first to last - 1 (the two ends may be cut)
*/
void list_shown(libscene_t const *scene, int *first, int *last)
{
    int bottom = (int)scene->scroll + scene->height;

    *first = (int)scene->scroll / LIST_STEP;
    *last = (bottom + LIST_STEP - 1) / LIST_STEP;
    if (*last > scene->nb_view)
        *last = scene->nb_view;
}

/*
** part of the screen holding the rows
*/
void list_area(libscene_t const *scene, float *rect)
{
    rect[0] = PANEL_X;
    rect[1] = LIST_Y;
    rect[2] = PANEL_W;
    rect[3] = scene->height;
}

/*
** the row at the height of the mouse, -1 if none
*/
int list_row_at(app_t *app)
{
    libscene_t *scene = app->libscene;
    float area[4];
    int row = 0;

    list_area(scene, area);
    if (app->mouse.y < area[1] || app->mouse.y >= area[1] + area[3])
        return -1;
    row = (app->mouse.y - LIST_Y + (int)scene->scroll) / LIST_STEP;
    return row < scene->nb_view ? row : -1;
}

/*
** handle of the slider; returns 0 if the whole list is shown
*/
static int handle_rect(libscene_t const *scene, float *rect)
{
    float max = scroll_max(scene);
    float height = scene->height;

    if (max <= 0)
        return 0;
    rect[0] = SLIDER_X;
    rect[2] = SLIDER_W;
    rect[3] = height * height / (scene->nb_view * LIST_STEP);
    if (rect[3] < SLIDER_MIN)
        rect[3] = SLIDER_MIN;
    rect[1] = LIST_Y + (height - rect[3]) * scene->scroll / max;
    return 1;
}

/*
** the handle is held where it was pressed; a click on the rest of the
** slider brings its middle under the mouse
*/
void slider_press(app_t *app)
{
    libscene_t *scene = app->libscene;
    float handle[4];
    float track[4] = {SLIDER_X - 8, LIST_Y, SLIDER_W + 16,
        scene->height};

    if (scene->sel.open || !handle_rect(scene, handle)
        || !mouse_in_rect(app, track))
        return;
    scene->slider_held = 1;
    scene->slider_grab = app->mouse.y - handle[1];
    if (scene->slider_grab < 0 || scene->slider_grab > handle[3])
        scene->slider_grab = handle[3] / 2;
}

/*
** returns 1 if the slider was held (the release is then not a click)
*/
int slider_release(app_t *app)
{
    int held = app->libscene->slider_held;

    app->libscene->slider_held = 0;
    return held;
}

static void slider_move(app_t *app, libscene_t *scene)
{
    float handle[4];
    float room = 0;

    if (!sfMouse_isButtonPressed(sfMouseLeft)
        || !handle_rect(scene, handle)) {
        scene->slider_held = 0;
        return;
    }
    room = scene->height - handle[3];
    if (room <= 0)
        return;
    scene->scroll_to = bounded((app->mouse.y - scene->slider_grab - LIST_Y)
        / room * scroll_max(scene), scroll_max(scene));
    scene->scroll = scene->scroll_to;
}

/*
** a part of the distance left is done at each tick, the same in a given
** time whatever the length of the tick
*/
void list_scroll_update(app_t *app)
{
    libscene_t *scene = app->libscene;
    float left = 0;

    if (scene->slider_held)
        slider_move(app, scene);
    left = scene->scroll_to - scene->scroll;
    scene->scroll += left * (1 - exp(-app->delta_us / SCROLL_US));
    if (fabs(scene->scroll_to - scene->scroll) < 1)
        scene->scroll = scene->scroll_to;
    app->tick_loop = scene->slider_held || scene->dragging
        || scene->scroll != scene->scroll_to ? TICK_SMOOTH : TICK_MS;
}

void slider_draw(app_t *app)
{
    libscene_t *scene = app->libscene;
    float handle[4];
    float track[4] = {SLIDER_X, LIST_Y, SLIDER_W, scene->height};
    sfColor back = {255, 255, 255, 30};
    sfColor idle = {255, 255, 255, 120};
    sfColor lit = {255, 200, 0, 220};

    if (!handle_rect(scene, handle))
        return;
    widget_rect(app, track, &back);
    widget_rect(app, handle, scene->slider_held
        || mouse_in_rect(app, handle) ? &lit : &idle);
}
