/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** Everything is placed in design coordinates (1920x1080).
** The view always shows at least that area, without distortion:
** - taller screens (16:10, 4:3) get extra height, added under the design
** - wider screens (21:9) get extra width, split on both sides
*/

#include "mp3player.h"

int view_create(app_t *app)
{
    sfFloatRect area = {0, 0, VIEW_W, VIEW_H};

    app->view = sfView_createFromRect(area);
    if (!app->view)
        return 84;
    view_update(app);
    return 0;
}

void view_update(app_t *app)
{
    sfVector2u size = sfRenderWindow_getSize(app->window);
    float ratio = (float)size.x / size.y;
    sfFloatRect area = {0, 0, VIEW_W, VIEW_H};

    if (ratio < (float)VIEW_W / VIEW_H) {
        area.height = VIEW_W / ratio;
    } else {
        area.width = VIEW_H * ratio;
        area.left = (VIEW_W - area.width) / 2;
    }
    sfView_reset(app->view, area);
    sfRenderWindow_setView(app->window, app->view);
    app->view_rect = area;
    app->extra_h = area.height - VIEW_H;
    app->relayout = 1;
}

void view_mouse(app_t *app)
{
    sfVector2i pixel = sfMouse_getPositionRenderWindow(app->window);

    app->mouse = sfRenderWindow_mapPixelToCoords(app->window, pixel,
        app->view);
}
