/*
** EPITECH PROJECT, 2025
** destroy_window.c
** File description:
** destroy all window elements
*/

#include "../../include/mp3player.h"

void destroy_window(window_t *win)
{
    if (win->window)
        sfRenderWindow_destroy(win->window);
    free(win);
}
