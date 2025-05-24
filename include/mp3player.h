/*
** EPITECH PROJECT, 2025
** mp3player.h
** File description:
** mp3player.h
*/

#ifndef MP3PLAYER_H_
    #define MP3PLAYER_H_

    #define WINDOW_TITLE "MP3_PLAYER"
    #define FRAMERATE_LIMIT 60
    #define MENU_TEXTURES 3
    #define LIST_TEXTURES 4

    #include "app.h"
    #include <stdlib.h>
    #include <unistd.h>
    #include <stddef.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <errno.h>
    #include <SFML/Graphics.h>
    #include <SFML/System.h>
    #include <SFML/Window.h>
    #include <SFML/Audio.h>
    #include <SFML/Network.h>
    #include <math.h>

// src
void isfreeable(char **arr, char *str);
int mouse_on_sprite(app_t *app, sfSprite *sprite);

// src/destroy
int destroy_all(app_t *app, int rtn_value);
void destroy_textures(texture_t *tx, state_t state);
void destroy_window(window_t *win);

// src/display
void draw_all(app_t *app);

// src/ini
app_t *init_all(void);
texture_t *init_textures(texture_t *tx, state_t state, window_t *win);
menu_t *init_menu(window_t *window);
plist_t *init_playlist(window_t *window);
window_t *init_window(void);

// src/update
int handle_events(app_t **app, sfEvent *event);
void handle_textures(app_t **app, sfEvent event);
int handle_window(app_t **app, sfEvent event);

#endif
