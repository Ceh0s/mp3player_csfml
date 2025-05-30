/*
** EPITECH PROJECT, 2025
** init textures
** File description:
** initialize all texture-related elements
*/

#include "../../include/mp3player.h"

texture_t *init_textures(texture_t *tx, state_t state, window_t *win)
{
    tx = malloc(sizeof(texture_t));
    if (state == MENU) {
        tx->menu = init_menu(win);
        tx->list = NULL;
        tx->music = NULL;
    }
    if (state == PLAYLIST) {
        tx->list = init_playlist(win);
        tx->music = init_music(win);
        tx->menu = NULL;
    }
    return tx;
}
