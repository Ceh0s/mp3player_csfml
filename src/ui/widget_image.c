/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** immediate mode widgets for the images: square (songs) or round
** (artists) like in YT Music, and the tooltip giving the name of the
** hovered image
*/

#include "mp3player.h"

/*
** rect = {x, y, w, h}; round draws a circle of diameter w
*/
void widget_image(app_t *app, sfTexture *texture, float const *rect,
    int round)
{
    sfRectangleShape *picture = app->wg->picture;
    sfCircleShape *circle = app->wg->circle;

    if (app->wg->mode != WG_DRAW || !texture)
        return;
    if (round) {
        sfCircleShape_setFillColor(circle, dark_tint(app));
        sfCircleShape_setTexture(circle, texture, sfTrue);
        sfCircleShape_setRadius(circle, rect[2] / 2);
        sfCircleShape_setPosition(circle, (sfVector2f){rect[0], rect[1]});
        sfRenderWindow_drawCircleShape(app->window, circle, NULL);
        return;
    }
    sfRectangleShape_setFillColor(picture, dark_tint(app));
    sfRectangleShape_setTexture(picture, texture, sfTrue);
    sfRectangleShape_setSize(picture, (sfVector2f){rect[2], rect[3]});
    sfRectangleShape_setPosition(picture, (sfVector2f){rect[0], rect[1]});
    sfRenderWindow_drawRectangleShape(app->window, picture, NULL);
}

/*
** str will be shown near the mouse if it is over rect
*/
void widget_tip(app_t *app, float const *rect, char const *str)
{
    if (app->wg->mode == WG_DRAW && mouse_in_rect(app, rect))
        snprintf(app->wg->tip, TIP_LEN, "%s", str);
}

/*
** to call at the end of the drawing of a scene, above everything
*/
void widget_tip_draw(app_t *app)
{
    sfColor back = {20, 20, 20, 235};
    float width = 0;
    float x = app->mouse.x + 18;
    float text[3] = {0, app->mouse.y + 23, 22};

    if (app->wg->tip[0] == '\0')
        return;
    width = widget_text_width(app, app->wg->tip, 22) + 24;
    if (x + width > app->view_rect.left + app->view_rect.width)
        x = app->mouse.x - width - 8;
    widget_rect(app, (float [4]){x, app->mouse.y + 18, width, 40}, &back);
    text[0] = x + 12;
    widget_text(app, app->wg->tip, text, &sfWhite);
    app->wg->tip[0] = '\0';
}
