/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** local window struct
*/

#ifndef WINDOW_H_
    #define WINDOW_H_

    #include <SFML/Window.h>

typedef struct window_s {
    sfRenderWindow *window;
    sfVideoMode mode;
    int width;
    int height;
} window_t;

#endif
