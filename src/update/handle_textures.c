/*
** EPITECH PROJECT, 2025
** handle textures
** File description:
** home to all texture-related events
*/

#include "../../include/mp3player.h"

static void change_state(app_t **app, int index)
{
    if (index == 1) {
        destroy_textures((*app)->tx, (*app)->state);
        (*app)->state = PLAYLIST;
        (*app)->tx = init_textures((*app)->tx, (*app)->state, (*app)->win);
    } else if (index == 2) {
        destroy_textures((*app)->tx, (*app)->state);
        (*app)->state = MENU;
        (*app)->tx = init_textures((*app)->tx, (*app)->state, (*app)->win);
    }
}

void handle_textures(app_t **app, sfEvent event)
{
    if (event.type == sfEvtMouseButtonReleased && (*app)->state == MENU &&
        event.mouseButton.button == sfMouseLeft && mouse_on_sprite(*app,
        (*app)->tx->menu->sprite[1]) == 1)
        change_state(app, 1);
    if (event.type == sfEvtMouseButtonReleased && (*app)->state == PLAYLIST
        && event.mouseButton.button == sfMouseLeft &&
        mouse_on_sprite(*app, (*app)->tx->list->sprite[3]) == 1)
        change_state(app, 2);
}
