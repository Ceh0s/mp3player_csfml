/*
** EPITECH PROJECT, 2025
** handle textures
** File description:
** home to all texture-related events
*/

#include "../../include/mp3player.h"

static int click_on_sprite(app_t *app)
{
    sfFloatRect rect;
    sfVector2i mousepos = sfMouse_getPositionRenderWindow(app->win->window);

    if (!app->tx->menu)
        return 0;
    rect = sfSprite_getGlobalBounds(app->tx->menu->sprite[1]);
    if (sfFloatRect_contains(&rect, (float)mousepos.x, (float)mousepos.y))
        return 1;
    return 0;
}

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
        event.mouseButton.button == sfMouseLeft && click_on_sprite(*app) == 1)
        change_state(app, 1);
    if (event.type == sfEvtMouseButtonReleased && (*app)->state == PLAYLIST
        && event.mouseButton.button == sfMouseLeft &&
        mouse_on_sprite(*app, (*app)->tx->list->sprite[3]) == 1 )
        change_state(app, 2);
}
