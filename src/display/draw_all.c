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

static void draw_skip_buttons(plist_t *list, window_t *win)
{
    sfRenderWindow_drawSprite(win->window, list->sprite[4], NULL);
    sfSprite_setRotation(list->sprite[4], 180);
    sfSprite_setPosition(list->sprite[4],
    (sfVector2f){((float)win->width / 2.50), ((float)win->height / 1.087)});
    sfRenderWindow_drawSprite(win->window, list->sprite[4], NULL);
    sfSprite_setRotation(list->sprite[4], 0);
    sfSprite_setPosition(list->sprite[4],
    (sfVector2f){((float)win->width / 1.66), ((float)win->height / 1.087)});
}

static void draw_music(music_t *music, window_t *win)
{
    for (int i = 0; music->text[i]; i++)
        sfRenderWindow_drawText(win->window, music->text[i], NULL);
}

static void draw_playlist(plist_t *list, music_t *music, window_t *win)
{
    sfRenderWindow_drawSprite(win->window, list->sprite[0], NULL);
    sfRenderWindow_drawRectangleShape(win->window, list->rect[0], NULL);
    draw_music(music, win);
    sfRenderWindow_drawRectangleShape(win->window, list->rect[1], NULL);
    sfRenderWindow_drawSprite(win->window, list->sprite[1], NULL);
    sfRenderWindow_drawSprite(win->window, list->sprite[2], NULL);
    sfRenderWindow_drawSprite(win->window, list->sprite[3], NULL);
    draw_skip_buttons(list, win);
}

void draw_all(app_t *app)
{
    if (app->state == MENU)
        draw_menu(app->tx->menu, app->win);
    if (app->state == PLAYLIST)
        draw_playlist(app->tx->list, app->tx->music, app->win);
}
