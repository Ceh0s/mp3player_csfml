/*
** EPITECH PROJECT, 2025
** destroy_all.c
** File description:
** destroys, clears & frees every element used in the program
*/

#include "../../include/mp3player.h"

int destroy_all(app_t *app, int rtn_value)
{
    if (!app)
        return rtn_value;
    destroy_window(app->win);
    if (app->tx)
        destroy_textures(app->tx, app->state);
    free(app);
    return rtn_value;
}
