/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** main struct pointer to all others *.h
*/

#ifndef APP_H_
    #define APP_H_

    #include "texture.h"
    #include "window.h"
    #include "music.h"
    #include <SFML/Window.h>

typedef enum state_s {
    MENU,
    PLAYLIST
} state_t;

typedef struct app_s {
    window_t *win;
    texture_t *tx;
    state_t state;
} app_t;

#endif
