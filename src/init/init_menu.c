/*
** EPITECH PROJECT, 2025
** init menu
** File description:
** initialize all menu-related elements
*/

#include "../../include/mp3player.h"

static menu_t *init_start_button(menu_t *menu, window_t *win)
{
    menu->texture[1] = sfTexture_createFromFile("img/playlogo.png", NULL);
    menu->sprite[1] = sfSprite_create();
    menu->scale[1] = (sfVector2f){((float)win->width / 3200),
    ((float)win->width / 3200)};
    sfSprite_setTexture(menu->sprite[1], menu->texture[1], sfTrue);
    sfSprite_setScale(menu->sprite[1], menu->scale[1]);
    sfSprite_setOrigin(menu->sprite[1], (sfVector2f){256, 256});
    sfSprite_setPosition(menu->sprite[1],
    (sfVector2f){((float)win->width / 2), ((float)win->height / 2)});
    return menu;
}

static menu_t *init_close_button(menu_t *menu, window_t *win)
{
    menu->texture[2] = sfTexture_createFromFile(
    "img/Cross_red_circle.png", NULL);
    menu->sprite[2] = sfSprite_create();
    menu->scale[2] = (sfVector2f){((float)win->width / 51200),
    ((float)win->width / 51200)};
    sfSprite_setTexture(menu->sprite[2], menu->texture[2], sfTrue);
    sfSprite_setScale(menu->sprite[2], menu->scale[2]);
    sfSprite_setOrigin(menu->sprite[2], (sfVector2f){1024, 1024});
    sfSprite_setPosition(menu->sprite[2],
    (sfVector2f){((float)win->width / 32), ((float)win->height / 20)});
    return menu;
}

static menu_t *init_background(menu_t *menu, window_t *win)
{
    menu->texture[0] = sfTexture_createFromFile(
    "img/rdm_wallpaper.jpg", NULL);
    menu->sprite[0] = sfSprite_create();
    menu->scale[0] = (sfVector2f){((float)win->width / 1980),
    ((float)win->height / 1080)};
    sfSprite_setTexture(menu->sprite[0], menu->texture[0], sfTrue);
    sfSprite_setScale(menu->sprite[0], menu->scale[0]);
    sfSprite_setPosition(menu->sprite[0], (sfVector2f){0.0, 0.0});
    return menu;
}

menu_t *init_menu(window_t *win)
{
    menu_t *menu = malloc(sizeof(menu_t));

    menu = init_background(menu, win);
    menu = init_start_button(menu, win);
    menu = init_close_button(menu, win);
    return menu;
}
