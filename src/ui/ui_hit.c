/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** mouse collisions, in design coordinates
*/

#include "mp3player.h"

int mouse_in_rect(app_t *app, float const *rect)
{
    return app->mouse.x >= rect[0] && app->mouse.x <= rect[0] + rect[2]
        && app->mouse.y >= rect[1] && app->mouse.y <= rect[1] + rect[3];
}

/*
** returns the id of the visible element under the mouse, -1 if none
*/
int ui_hit(app_t *app, ui_t *ui)
{
    for (int i = 0; i < ui->nb; i++)
        if (ui->visible[i] && mouse_in_rect(app, ui->rects[i]))
            return ui->defs[i].id;
    return -1;
}
