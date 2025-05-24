/*
** EPITECH PROJECT, 2025
** texture.h
** File description:
** local texture structs
*/

#ifndef TEXTURE_H_
    #define TEXTURE_H_

    #include <SFML/Graphics.h>

typedef struct menu_s {
    sfTexture *texture[3];
    sfSprite *sprite[3];
    sfVector2f scale[3];
} menu_t;

typedef struct plist_s {
    sfTexture *texture[4];
    sfSprite *sprite[4];
    sfVector2f scale[4];
    sfRectangleShape *rect;
} plist_t;

typedef struct texture_s {
    menu_t *menu;
    plist_t *list;
} texture_t;

#endif
