/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** drag and drop of a song in the custom order of a playlist: pressed on
** a row, moved by more than 10 pixels, released where it goes. A simple
** click still plays the song; in the other orders nothing is dragged.
*/

#include "mp3player.h"

static int is_custom(app_t *app)
{
    view_sort_t *sort = sort_get(app->lib, app->libscene->playlist_id);

    return sort && sort->key == SORT_CUSTOM;
}

/*
** insertion place under the mouse: before the row whose upper half
** holds it, kept inside the rows shown
*/
static int drop_place(app_t *app)
{
    libscene_t *scene = app->libscene;
    float pos = (app->mouse.y - LIST_Y + LIST_STEP / 2.0) / LIST_STEP;
    int place = scene->scroll + (pos < 0 ? 0 : (int)pos);
    int last = scene->scroll + scene->rows;

    if (last > scene->nb_view)
        last = scene->nb_view;
    return place > last ? last : place;
}

/*
** the song leaves its row and is inserted before the row _to_ (counted
** before it left), then the view becomes the custom order
*/
static void move_song(app_t *app, int from, int to)
{
    libscene_t *scene = app->libscene;
    view_sort_t *sort = sort_get(app->lib, scene->playlist_id);
    song_t *moved = scene->view[from];

    if (to > from)
        to--;
    if (!sort || to == from)
        return;
    if (to > from)
        memmove(&scene->view[from], &scene->view[from + 1],
            sizeof(song_t *) * (to - from));
    else
        memmove(&scene->view[to + 1], &scene->view[to],
            sizeof(song_t *) * (from - to));
    scene->view[to] = moved;
    if (sort_set_custom(app->lib, sort, scene->view, scene->nb_view) == 84)
        return;
    db_save_sorts(app->lib);
    library_reorder(app, 1);
}

void drag_press(app_t *app)
{
    libscene_t *scene = app->libscene;

    scene->drag_row = is_custom(app) ? list_click(app) : -1;
    scene->press = app->mouse;
    scene->dragging = 0;
    scene->drag_wait = 0;
}

/*
** above or under the list, it scrolls by one row every 4 ticks
*/
static void auto_scroll(app_t *app, libscene_t *scene)
{
    if (scene->drag_wait > 0) {
        scene->drag_wait--;
        return;
    }
    scene->drag_wait = 4;
    if (app->mouse.y < LIST_Y)
        list_scroll(app, 1);
    else if (app->mouse.y > LIST_Y + scene->rows * LIST_STEP)
        list_scroll(app, -1);
}

void drag_update(app_t *app)
{
    libscene_t *scene = app->libscene;
    float dx = app->mouse.x - scene->press.x;
    float dy = app->mouse.y - scene->press.y;

    if (scene->drag_row < 0)
        return;
    if (!sfMouse_isButtonPressed(sfMouseLeft)) {
        scene->drag_row = -1;
        scene->dragging = 0;
        return;
    }
    if (dx * dx + dy * dy > 100)
        scene->dragging = 1;
    if (scene->dragging)
        auto_scroll(app, scene);
}

/*
** returns 1 if a song was dragged (the release is then not a click)
*/
int drag_release(app_t *app)
{
    libscene_t *scene = app->libscene;
    int from = scene->drag_row;
    int dragged = scene->dragging;

    scene->drag_row = -1;
    scene->dragging = 0;
    if (from < 0 || from >= scene->nb_view || !dragged)
        return 0;
    move_song(app, from, drop_place(app));
    return 1;
}

/*
** the row held is lit, a line shows where it will be inserted
*/
void drag_draw(app_t *app)
{
    libscene_t *scene = app->libscene;
    sfColor held = {255, 200, 0, 60};
    sfColor line = {255, 200, 0, 255};
    int row = scene->drag_row - scene->scroll;
    float rect[4] = {LIST_X - 10, LIST_Y + row * LIST_STEP, 1690, LIST_STEP};

    if (scene->drag_row < 0 || !scene->dragging)
        return;
    if (row >= 0 && row < scene->rows)
        widget_rect(app, rect, &held);
    rect[1] = LIST_Y + (drop_place(app) - scene->scroll) * LIST_STEP - 2;
    rect[3] = 4;
    widget_rect(app, rect, &line);
}
