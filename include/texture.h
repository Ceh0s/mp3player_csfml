/*
** EPITECH PROJECT, 2025
** texture.h
** File description:
** local texture structs
*/

#ifndef TEXTURE_H_
    #define TEXTURE_H_

    #include "music.h"
    #include <SFML/Graphics.h>

typedef struct menu_s {
    sfTexture *texture[MENU_TEXTURES];
    sfSprite *sprite[MENU_TEXTURES];
    sfVector2f scale[MENU_TEXTURES];
} menu_t;

typedef struct plist_s {
    sfTexture *texture[LIST_TEXTURES];
    sfSprite *sprite[LIST_TEXTURES];
    sfVector2f scale[LIST_TEXTURES];
    sfRectangleShape *rect[2];
} plist_t;

typedef struct texture_s {
    menu_t *menu;
    plist_t *list;
    music_t *music;
} texture_t;

#endif
