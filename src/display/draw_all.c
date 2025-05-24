/*
** EPITECH PROJECT, 2025
** draw_all
** File description:
** main texture display function
*/

#include "../../include/mp3player.h"

static void draw_menu(menu_t *menu, sfRenderWindow *window)
{
    sfRenderWindow_drawSprite(window, menu->sprite[0], NULL);
    sfRenderWindow_drawSprite(window, menu->sprite[1], NULL);
    sfRenderWindow_drawSprite(window, menu->sprite[2], NULL);
}

static void draw_playlist(plist_t *list, sfRenderWindow *window)
{
    sfRenderWindow_drawSprite(window, list->sprite[0], NULL);
    sfRenderWindow_drawRectangleShape(window, list->rect, NULL);
    sfRenderWindow_drawSprite(window, list->sprite[1], NULL);
    sfRenderWindow_drawSprite(window, list->sprite[2], NULL);
    sfRenderWindow_drawSprite(window, list->sprite[3], NULL);
}

void draw_all(app_t *app)
{
    if (app->state == MENU)
        draw_menu(app->tx->menu, app->win->window);
    if (app->state == PLAYLIST)
        draw_playlist(app->tx->list, app->win->window);
}
