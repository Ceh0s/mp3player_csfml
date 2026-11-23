/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** help shown when the mouse is over a button that does not explain
** itself, only if "Aide au survol" is on (settings screen)
*/

#include "mp3player.h"

void widget_help(app_t *app, float const *rect, char const *str)
{
    if (app->lib->settings.help)
        widget_tip(app, rect, str);
}
