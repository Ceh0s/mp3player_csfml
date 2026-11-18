/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** window creation and fullscreen toggle (F11)
*/

#include "mp3player.h"

int window_create(app_t *app)
{
    sfVideoMode mode = {WINDOWED_W, WINDOWED_H, 32};
    sfUint32 style = sfDefaultStyle;

    if (app->fullscreen) {
        mode = sfVideoMode_getDesktopMode();
        style = sfFullscreen;
    }
    app->window = sfRenderWindow_create(mode, WINDOW_TITLE, style, NULL);
    if (!app->window)
        return 84;
    sfRenderWindow_setKeyRepeatEnabled(app->window, sfFalse);
    if (app->view)
        view_update(app);
    return 0;
}

/*
** Done after the event loop: destroying the window while polling
** its events would read freed memory.
*/
void window_apply_toggle(app_t *app)
{
    if (!app->toggle_fullscreen)
        return;
    app->toggle_fullscreen = 0;
    app->fullscreen = !app->fullscreen;
    sfRenderWindow_destroy(app->window);
    if (window_create(app) == 84) {
        app->fullscreen = !app->fullscreen;
        window_create(app);
    }
}
