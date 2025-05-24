/*
** EPITECH PROJECT, 2025
** init all
** File description:
** lobby for initializing every element in structures
*/

#include "../../include/mp3player.h"

app_t *init_all(void)
{
    app_t *app = malloc(sizeof(app_t));

    if (!app)
        return NULL;
    app->state = MENU;
    app->win = init_window();
    app->tx = init_textures(NULL, app->state, app->win);
    return app;
}
