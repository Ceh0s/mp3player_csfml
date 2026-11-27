/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** more immediate mode widgets: colored rectangle, width of a text
** (they reuse the single sfText and rectangle of the widgets)
*/

#include "mp3player.h"

/*
** rect = {x, y, w, h}
*/
void widget_rect(app_t *app, float const *rect, sfColor const *color)
{
    sfRectangleShape *box = app->wg->box;

    if (app->wg->mode != WG_DRAW)
        return;
    sfRectangleShape_setPosition(box, (sfVector2f){rect[0], rect[1]});
    sfRectangleShape_setSize(box, (sfVector2f){rect[2], rect[3]});
    sfRectangleShape_setFillColor(box, dark_soften(app, *color));
    sfRectangleShape_setOutlineThickness(box, 0);
    sfRenderWindow_drawRectangleShape(app->window, box, NULL);
}

/*
** width in design pixels of str written at this character size
*/
float widget_text_width(app_t *app, char const *str, unsigned int size)
{
    text_set(app->wg->text, str);
    sfText_setCharacterSize(app->wg->text, size);
    return text_width(app, app->wg->text);
}

/*
** a disc of this color centered on (x, y)
*/
void widget_dot(app_t *app, float x, float y, float radius,
    sfColor const *color)
{
    sfCircleShape *circle = app->wg->circle;

    if (app->wg->mode != WG_DRAW)
        return;
    sfCircleShape_setTexture(circle, NULL, sfFalse);
    sfCircleShape_setFillColor(circle, dark_soften(app, *color));
    sfCircleShape_setRadius(circle, radius);
    sfCircleShape_setPosition(circle, (sfVector2f){x - radius, y - radius});
    sfRenderWindow_drawCircleShape(app->window, circle, NULL);
}
