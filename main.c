/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** main of the mp3 player
*/

#include "mp3player.h"

int main(void)
{
    app_t *app = app_create();

    if (!app)
        return 84;
    while (sfRenderWindow_isOpen(app->window))
        if (loop_run(app) == 84)
            return app_destroy(app, 84);
    return app_destroy(app, 0);
}
