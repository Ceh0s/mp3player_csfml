/*
** EPITECH PROJECT, 2025
** init menu
** File description:
** initialize all menu-related elements
*/

#include "../../include/mp3player.h"

static plist_t *init_start_button(plist_t *list, window_t *win)
{
    list->texture[1] = sfTexture_createFromFile("img/playlogo.png", NULL);
    list->sprite[1] = sfSprite_create();
    list->scale[1] = (sfVector2f){((float)win->width / 8533),
    ((float)win->width / 8533)};
    sfSprite_setTexture(list->sprite[1], list->texture[1], sfTrue);
    sfSprite_setScale(list->sprite[1], list->scale[1]);
    sfSprite_setOrigin(list->sprite[1], (sfVector2f){256, 256});
    sfSprite_setPosition(list->sprite[1],
    (sfVector2f){((float)win->width / 2), ((float)win->height / 1.1)});
    return list;
}

static plist_t *init_close_button(plist_t *list, window_t *win)
{
    list->texture[2] = sfTexture_createFromFile(
    "img/Cross_red_circle.png", NULL);
    list->sprite[2] = sfSprite_create();
    list->scale[2] = (sfVector2f){((float)win->width / 51200),
    ((float)win->width / 51200)};
    sfSprite_setTexture(list->sprite[2], list->texture[2], sfTrue);
    sfSprite_setScale(list->sprite[2], list->scale[2]);
    sfSprite_setOrigin(list->sprite[2], (sfVector2f){1024, 1024});
    sfSprite_setPosition(list->sprite[2],
    (sfVector2f){((float)win->width / 32), ((float)win->height / 20)});
    return list;
}

static plist_t *init_back_button(plist_t *list, window_t *win)
{
    list->texture[3] = sfTexture_createFromFile(
    "img/red_left_arrow.png", NULL);
    list->sprite[3] = sfSprite_create();
    list->scale[3] = (sfVector2f){((float)win->width / 12800),
    ((float)win->width / 12800)};
    sfSprite_setTexture(list->sprite[3], list->texture[3], sfTrue);
    sfSprite_setScale(list->sprite[3], list->scale[3]);
    sfSprite_setOrigin(list->sprite[3], (sfVector2f){256, 256});
    sfSprite_setPosition(list->sprite[3],
    (sfVector2f){((float)win->width / 12), ((float)win->height / 20)});
    return list;
}

static plist_t *init_skip_button(plist_t *list, window_t *win)
{
    list->texture[4] = sfTexture_createFromFile(
    "img/skip_button.png", NULL);
    list->sprite[4] = sfSprite_create();
    list->scale[4] = (sfVector2f){((float)win->width / 10240),
    ((float)win->width / 10240)};
    sfSprite_setTexture(list->sprite[4], list->texture[4], sfTrue);
    sfSprite_setScale(list->sprite[4], list->scale[4]);
    sfSprite_setOrigin(list->sprite[4], (sfVector2f){235, 203});
    sfSprite_setPosition(list->sprite[4],
    (sfVector2f){((float)win->width / 1.66), ((float)win->height / 1.1)});
    return list;
}

static plist_t *init_background(plist_t *list, window_t *win)
{
    list->texture[0] = sfTexture_createFromFile(
    "img/rdm_wallpaper_blurred.png", NULL);
    list->sprite[0] = sfSprite_create();
    list->scale[0] = (sfVector2f){((float)win->width / 1980),
    ((float)win->height / 1080)};
    sfSprite_setTexture(list->sprite[0], list->texture[0], sfTrue);
    sfSprite_setScale(list->sprite[0], list->scale[0]);
    sfSprite_setPosition(list->sprite[0], (sfVector2f){0.0, 0.0});
    return list;
}

static plist_t *init_rect(plist_t *list, window_t *win)
{
    sfVector2f size = (sfVector2f){((float)win->width / 1.2),
    ((float)win->height / 1.2)};

    list->rect = sfRectangleShape_create();
    sfRectangleShape_setSize(list->rect, size);
    sfRectangleShape_setFillColor(list->rect, (sfColor){255, 255, 255, 100});
    sfRectangleShape_setOrigin(list->rect, (sfVector2f){0.0, 0.0});
    sfRectangleShape_setPosition(list->rect,
    (sfVector2f){size.x / 10, size.y / 10});
    return list;
}

plist_t *init_playlist(window_t *win)
{
    plist_t *list = malloc(sizeof(plist_t));

    list = init_background(list, win);
    list = init_start_button(list, win);
    list = init_close_button(list, win);
    list = init_back_button(list, win);
    list = init_skip_button(list, win);
    list = init_rect(list, win);
    return list;
}
