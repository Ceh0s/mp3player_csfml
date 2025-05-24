/*
** EPITECH PROJECT, 2025
** destroy_textures.c
** File description:
** destroy all texture elements
*/

#include "../../include/mp3player.h"

void destroy_menu(menu_t *menu)
{
    for (int i = 0; i < MENU_TEXTURES; i++)
        if (menu->texture[i])
            sfTexture_destroy(menu->texture[i]);
    for (int i = 0; i < MENU_TEXTURES; i++)
        if (menu->sprite[i])
            sfSprite_destroy(menu->sprite[i]);
    free(menu);
}

void destroy_playlist(plist_t *list)
{
    for (int i = 0; i < LIST_TEXTURES; i++)
        if (list->texture[i])
            sfTexture_destroy(list->texture[i]);
    for (int i = 0; i < LIST_TEXTURES; i++)
        if (list->sprite[i])
            sfSprite_destroy(list->sprite[i]);
    if (list->rect)
        sfRectangleShape_destroy(list->rect);
    free(list);
}

void destroy_textures(texture_t *tx, state_t state)
{
    if (!tx)
        return;
    if (state == MENU)
        if (tx->menu)
            destroy_menu(tx->menu);
    if (state == PLAYLIST)
        if (tx->list)
            destroy_playlist(tx->list);
    free(tx);
}
