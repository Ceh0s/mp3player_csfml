/*
** EPITECH PROJECT, 2025
** init window
** File description:
** initialize all window-related elements
*/

#include "../../include/mp3player.h"

window_t *init_window(void)
{
    window_t *win = malloc(sizeof(window_t));

    win->mode = sfVideoMode_getDesktopMode();
    win->mode.bitsPerPixel = 32;
    win->window = sfRenderWindow_create(win->mode,
    WINDOW_TITLE, sfClose | sfFullscreen, NULL);
    if (!win->window)
        return NULL;
    win->width = win->mode.width;
    win->height = win->mode.height;
    sfRenderWindow_setFramerateLimit(win->window, FRAMERATE_LIMIT);
    return win;
}
