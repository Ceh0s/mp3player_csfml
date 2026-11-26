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
** rect = {x, y, w, h} of a row
*/
static void row_rect(libscene_t const *scene, int row, float *rect)
{
    rect[0] = LIST_X;
    rect[1] = list_row_y(scene, row);
    rect[2] = LIST_RIGHT - LIST_X;
    rect[3] = LIST_STEP;
}

/*
** the row under the mouse (-1 if none); the like icons of a row are not
** part of it, nor is the list while the sort criteria are open
*/
int list_click(app_t *app)
{
    if (app->libscene->sort_open || app->libscene->list.held
        || app->mouse.x < LIST_X || app->mouse.x > LIST_LIKES_X - 8)
        return -1;
    return list_row_at(app);
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
    int first = 0;
    int last = 0;

    list_shown(scene, &first, &last);
    for (int i = first; i < last; i++) {
        rank.song = scene->view[i];
        thumb[1] = list_row_y(scene, i) + 4;
        text[1] = thumb[1] + 2;
        widget_image(app, image_get(app, &rank, 1), thumb, 0);
        widget_song(app, rank.song, text, row_color(app, i, over));
    }
}

/*
** the list follows when an automatic playlist ("Titres likés") is shown
*/
static void row_likes(app_t *app, libscene_t *scene)
{
    playlist_t *shown = playlist_by_id(app->lib, scene->playlist_id);
    float rect[4];
    int first = 0;
    int last = 0;

    list_shown(scene, &first, &last);
    for (int i = first; i < last; i++) {
        row_rect(scene, i, rect);
        if (!like_row(app, scene->view[i], rect, LIST_LIKES_X))
            continue;
        if (shown && shown->auto_like != 0)
            list_refresh(app);
        return;
    }
}

/*
** drawn and clicked with the other widgets; the part of a row cut by
** the top or the bottom of the list is not clicked
*/
void list_likes(app_t *app)
{
    float area[4];

    if (app->libscene->sort_open)
        return;
    if (app->wg->mode != WG_DRAW) {
        if (list_row_at(app) != -1)
            row_likes(app, app->libscene);
        return;
    }
    list_area(app->libscene, area);
    clip_begin(app, area);
    row_likes(app, app->libscene);
    clip_end(app);
}
