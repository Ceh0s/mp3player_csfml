/*
** EPITECH PROJECT, 2025
** handle event
** File description:
** handles csfml related events except the window
*/

#include "../../include/mp3player.h"

int handle_events(app_t **app, sfEvent *event)
{
    while (sfRenderWindow_pollEvent((*app)->win->window, event)) {
        if ((*app)->tx)
            handle_textures(app, *event);
        if ((*app)->tx->music)
            update_music(app, *event);
        if (handle_window(app, *event) == 84)
            return 84;
    }
    return 0;
}
