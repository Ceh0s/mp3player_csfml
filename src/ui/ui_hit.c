/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** mouse collisions, in design coordinates
*/

#include "mp3player.h"

static int in_rect(sfVector2f mouse, float const *rect)
{
    return mouse.x >= rect[0] && mouse.x <= rect[0] + rect[2]
        && mouse.y >= rect[1] && mouse.y <= rect[1] + rect[3];
}

/*
** the part of a list hidden by its cut (clip.c) is under nothing
*/
int mouse_in_rect(app_t *app, float const *rect)
{
    if (app->wg && app->wg->clip_on && !in_rect(app->mouse, app->wg->clip_area))
        return 0;
    return in_rect(app->mouse, rect);
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
