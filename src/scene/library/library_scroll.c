/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scroll of the list of the library, in pixels (src/ui/scroll.c): the
** wheel, the slider and the drag of a song move it; the rows under the
** mouse are found from it.
*/

#include "mp3player.h"

/*
** pixels > 0 goes down the list; 0 only keeps the scroll valid (after a
** resize or a change of the list)
*/
void list_scroll(app_t *app, float pixels)
{
    libscene_t *scene = app->libscene;
    float area[4];

    list_area(scene, area);
    scroll_frame(&scene->list, area, scene->nb_view * LIST_STEP);
    scroll_by(&scene->list, pixels);
}

/*
** y of a row of the list, as it is shown now (it may be partly, or
** fully, out of the list)
*/
float list_row_y(libscene_t const *scene, int row)
{
    return scroll_y(&scene->list, row * LIST_STEP);
}

/*
** the rows shown are first to last - 1 (the two ends may be cut)
*/
void list_shown(libscene_t const *scene, int *first, int *last)
{
    int bottom = (int)scene->list.pos + scene->height;

    *first = (int)scene->list.pos / LIST_STEP;
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
    row = (app->mouse.y - LIST_Y + (int)scene->list.pos) / LIST_STEP;
    return row < scene->nb_view ? row : -1;
}

/*
** the slider cannot be taken while the actions window covers the list;
** the drag of a song also scrolls (drawn faster)
*/
void list_scroll_update(app_t *app)
{
    libscene_t *scene = app->libscene;

    if (!scene->sel.open)
        scroll_grab(app, &scene->list);
    scroll_update(app, &scene->list);
    if (scene->dragging)
        app->tick_loop = TICK_SMOOTH;
}
