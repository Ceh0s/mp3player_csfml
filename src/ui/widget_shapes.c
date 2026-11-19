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
    sfRectangleShape_setFillColor(box, *color);
    sfRectangleShape_setOutlineThickness(box, 0);
    sfRenderWindow_drawRectangleShape(app->window, box, NULL);
}

/*
** width in design pixels of str written at this character size
*/
float widget_text_width(app_t *app, char const *str, unsigned int size)
{
    sfFloatRect bounds;

    text_set(app->wg->text, str);
    sfText_setCharacterSize(app->wg->text, size);
    bounds = sfText_getLocalBounds(app->wg->text);
    return bounds.width;
}
