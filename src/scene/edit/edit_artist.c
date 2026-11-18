/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** artist page opened from the edit screen (the edits in progress are
** kept, since the edit scene stays loaded)
*/

#include "mp3player.h"

void edit_artist(app_t *app)
{
    int back = 0;

    artist_page(app, app->edit->artist, &back);
    if (back)
        app->edit->mode = EDIT_MAIN;
}
