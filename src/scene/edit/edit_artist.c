/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** artist page opened from the edit screen (the edits in progress are
** kept, since the edit scene stays loaded), with the choice of the
** image of the artist (round, like in YT Music)
*/

#include "mp3player.h"

static void image(app_t *app, edit_t *edit)
{
    rank_t rank = {NULL, edit->artist, 0, 0, 0};

    widget_image(app, image_get(app, &rank, 0),
        (float [4]){1430, 130, 260, 260}, 1);
    edit->image.focused = edit->focus == FOCUS_IMAGE;
    if (image_box(app, &rank, &edit->image,
            (float [3]){1300, 420, 480}))
        edit->focus = FOCUS_IMAGE;
}

void edit_artist(app_t *app)
{
    int back = 0;

    artist_page(app, app->edit->artist, &back);
    if (app->edit->artist)
        image(app, app->edit);
    if (!back)
        return;
    app->edit->mode = EDIT_MAIN;
    app->edit->focus = FOCUS_TITLE;
    app->edit->image.msg[0] = '\0';
}
