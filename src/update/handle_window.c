/*
** EPITECH PROJECT, 2025
** handle_window
** File description:
** checks window status through events & edit elements accordingly
*/

#include "../../include/mp3player.h"

static int is_cross_sprite(app_t *app)
{
    if (app->tx->list)
        return mouse_on_sprite(app, app->tx->list->sprite[2]);
    if (app->tx->menu)
        return mouse_on_sprite(app, app->tx->menu->sprite[2]);
    return 0;
}

int handle_window(app_t **app, sfEvent event)
{
    if (!(*app)->win->window)
        return 84;
    sfRenderWindow_clear((*app)->win->window, sfBlack);
    if (event.type == sfEvtClosed || (event.type == sfEvtMouseButtonReleased
        && event.mouseButton.button == sfMouseLeft
        && is_cross_sprite(*app) == 1))
        sfRenderWindow_close((*app)->win->window);
    return 0;
}
