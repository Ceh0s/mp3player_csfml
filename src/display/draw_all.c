/*
** EPITECH PROJECT, 2025
** draw_all
** File description:
** main texture display function
*/

#include "../../include/mp3player.h"

static void draw_menu(menu_t *menu, window_t *win)
{
    sfRenderWindow_drawSprite(win->window, menu->sprite[0], NULL);
    sfRenderWindow_drawSprite(win->window, menu->sprite[1], NULL);
    sfRenderWindow_drawSprite(win->window, menu->sprite[2], NULL);
}

static void draw_playlist(plist_t *list, music_t *music, window_t *win)
{
    sfRenderWindow_drawSprite(win->window, list->sprite[0], NULL);
    sfRenderWindow_drawRectangleShape(win->window, list->rect[0], NULL);
    for (int i = 0; music->text[i]; i++)
        sfRenderWindow_drawText(win->window, music->text[i], NULL);
    sfRenderWindow_drawRectangleShape(win->window, list->rect[1], NULL);
    if (music->paused == 1)
        sfRenderWindow_drawSprite(win->window, list->sprite[1], NULL);
    else
        sfRenderWindow_drawSprite(win->window, list->sprite[5], NULL);
    sfRenderWindow_drawSprite(win->window, list->sprite[2], NULL);
    sfRenderWindow_drawSprite(win->window, list->sprite[3], NULL);
    sfRenderWindow_drawSprite(win->window, list->sprite[4], NULL);
    sfRenderWindow_drawSprite(win->window, list->sprite[6], NULL);
}

void draw_all(app_t *app)
{
    if (app->state == MENU)
        draw_menu(app->tx->menu, app->win);
    if (app->state == PLAYLIST)
        draw_playlist(app->tx->list, app->tx->music, app->win);
}
