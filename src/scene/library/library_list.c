/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scrollable list of the songs of the library screen: each row shows
** the small image of its song, its title with its artists under it, and
** its like (both like icons while the mouse is over the row)
*/

#include "mp3player.h"

static const sfColor hover = {200, 200, 255, 255};

/*
** rect = {x, y, w, h} of the row shown at the position pos (0 = first)
*/
static void row_rect(int pos, float *rect)
{
    rect[0] = LIST_X;
    rect[1] = LIST_Y + pos * LIST_STEP;
    rect[2] = LIST_RIGHT - LIST_X;
    rect[3] = LIST_STEP;
}

/*
** the row under the mouse (-1 if none); the like icons of a row are not
** part of it, nor is the list while the sort criteria are open
*/
int list_click(app_t *app)
{
    libscene_t *scene = app->libscene;
    int row = 0;

    if (scene->sort_open || app->mouse.y < LIST_Y
        || app->mouse.x < LIST_X || app->mouse.x > LIST_LIKES_X - 8)
        return -1;
    row = (app->mouse.y - LIST_Y) / LIST_STEP;
    if (row >= scene->rows || scene->scroll + row >= scene->nb_view)
        return -1;
    return scene->scroll + row;
}

/*
** the tab shown becomes the queue; the song playing is paused instead
*/
void list_play(app_t *app, int row)
{
    player_queue(app, app->libscene->playlist_id, app->search.text);
    if (app->libscene->view[row] == app->player->song)
        player_toggle(app);
    else
        player_play(app, row);
}

void list_scroll(app_t *app, float delta)
{
    int max = app->libscene->nb_view - app->libscene->rows;
    int scroll = app->libscene->scroll - (int)delta;

    if (scroll > max)
        scroll = max;
    if (scroll < 0)
        scroll = 0;
    app->libscene->scroll = scroll;
}

/*
** the song playing is yellow, the one under the mouse light blue
*/
static sfColor const *row_color(app_t *app, int i, int over)
{
    if (app->libscene->view[i] == app->player->song)
        return &sfYellow;
    return i == over ? &hover : &sfWhite;
}

void list_draw(app_t *app)
{
    libscene_t *scene = app->libscene;
    rank_t rank = {NULL, NULL, 0, 0, 0};
    float thumb[4] = {LIST_X, 0, LIST_THUMB, LIST_THUMB};
    float text[4] = {LIST_TEXT_X, 0, LIST_LIKES_X - LIST_TEXT_X - 20,
        LIST_STEP - 8};
    int over = list_click(app);

    for (int i = scene->scroll; i < scene->nb_view
        && i < scene->scroll + scene->rows; i++) {
        rank.song = scene->view[i];
        thumb[1] = LIST_Y + (i - scene->scroll) * LIST_STEP + 4;
        text[1] = thumb[1] + 2;
        widget_image(app, image_get(app, &rank, 1), thumb, 0);
        widget_song(app, rank.song, text, row_color(app, i, over));
    }
}

/*
** drawn and clicked with the other widgets; the list follows when an
** automatic playlist ("Titres likés") is shown
*/
void list_likes(app_t *app)
{
    libscene_t *scene = app->libscene;
    playlist_t *shown = playlist_by_id(app->lib, scene->playlist_id);
    float rect[4];

    if (scene->sort_open)
        return;
    for (int i = scene->scroll; i < scene->nb_view
        && i < scene->scroll + scene->rows; i++) {
        row_rect(i - scene->scroll, rect);
        if (!like_row(app, scene->view[i], rect, LIST_LIKES_X))
            continue;
        if (shown && shown->auto_like != 0)
            list_create(app);
        return;
    }
}
