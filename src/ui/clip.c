/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** nothing is drawn out of a rectangle between clip_begin and clip_end
** (the rows of a list cut at its top and bottom): a view showing only
** this part of the design, placed at the same spot of the window
*/

#include "mp3player.h"

/*
** rect = {x, y, w, h}, in design coordinates
*/
void clip_begin(app_t *app, float const *rect)
{
    sfFloatRect all = app->view_rect;
    sfFloatRect area = {rect[0], rect[1], rect[2], rect[3]};
    sfFloatRect part = {(rect[0] - all.left) / all.width,
        (rect[1] - all.top) / all.height, rect[2] / all.width,
        rect[3] / all.height};

    app->wg->clip_on = 1;
    memcpy(app->wg->clip_area, rect, sizeof(app->wg->clip_area));
    sfView_reset(app->wg->clip, area);
    sfView_setViewport(app->wg->clip, part);
    sfRenderWindow_setView(app->window, app->wg->clip);
}

void clip_end(app_t *app)
{
    app->wg->clip_on = 0;
    sfRenderWindow_setView(app->window, app->view);
}
