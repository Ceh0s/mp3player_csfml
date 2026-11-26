/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** helpers creating texts, rectangles and backgrounds
*/

#include "mp3player.h"

sfText *text_create(app_t *app, unsigned int size, float x, float y)
{
    sfText *text = sfText_create();
    sfVector2f pos = {x, y};

    if (!text)
        return NULL;
    sfText_setFont(text, app->font);
    sfText_setCharacterSize(text, size);
    sfText_setFillColor(text, sfWhite);
    sfText_setPosition(text, pos);
    return text;
}

void text_set(sfText *text, char const *str)
{
    sfUint32 *unicode = utf8_to_utf32(str);

    if (!unicode)
        return;
    sfText_setUnicodeString(text, unicode);
    free(unicode);
}

/*
** rect = {x, y, w, h}
*/
sfRectangleShape *rect_create(float const *rect, sfColor const *color)
{
    sfRectangleShape *shape = sfRectangleShape_create();
    sfVector2f pos = {rect[0], rect[1]};
    sfVector2f size = {rect[2], rect[3]};

    if (!shape)
        return NULL;
    sfRectangleShape_setPosition(shape, pos);
    sfRectangleShape_setSize(shape, size);
    sfRectangleShape_setFillColor(shape, *color);
    return shape;
}

sfSprite *background_create(char const *path, sfTexture **texture)
{
    sfSprite *sprite = NULL;

    *texture = sfTexture_createFromFile(path, NULL);
    if (!*texture)
        return NULL;
    sprite = sfSprite_create();
    if (!sprite)
        return NULL;
    sfSprite_setTexture(sprite, *texture, sfTrue);
    return sprite;
}

/*
** nothing in dark mode: the window is already cleared in black
*/
void background_draw(app_t *app, sfSprite *sprite)
{
    if (!app->lib->settings.dark)
        sfRenderWindow_drawSprite(app->window, sprite, NULL);
}

/*
** the background covers the whole view without being distorted:
** same scale on both axes, centered, the overflow is cropped
*/
void background_layout(app_t *app, sfSprite *sprite)
{
    sfFloatRect view = app->view_rect;
    sfVector2u size = sfTexture_getSize(sfSprite_getTexture(sprite));
    float scale = view.width / size.x;
    sfVector2f pos;

    if (view.height / size.y > scale)
        scale = view.height / size.y;
    pos.x = view.left + (view.width - size.x * scale) / 2;
    pos.y = view.top + (view.height - size.y * scale) / 2;
    sfSprite_setScale(sprite, (sfVector2f){scale, scale});
    sfSprite_setPosition(sprite, pos);
}
