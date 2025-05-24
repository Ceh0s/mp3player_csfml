/*
** EPITECH PROJECT, 2025
** mouse on sprite
** File description:
** returns 1 if mouse coordinates is within given sprite bounds
*/

#include "../include/mp3player.h"

int mouse_on_sprite(app_t *app, sfSprite *sprite)
{
    sfFloatRect rect = sfSprite_getGlobalBounds(sprite);
    sfVector2i mousepos = sfMouse_getPositionRenderWindow(app->win->window);

    if (sfFloatRect_contains(&rect, (float)mousepos.x, (float)mousepos.y))
        return 1;
    return 0;
}

