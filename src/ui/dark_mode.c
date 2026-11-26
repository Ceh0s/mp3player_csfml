/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** dark mode (settings screen): no background picture (black), softer
** colors for the texts, icons, buttons and images; the elements that were
** already black are inverted where they are drawn
*/

#include "mp3player.h"

/*
** in dark mode a color is 80 % as bright (white becomes light gray), its
** transparency is kept; unchanged otherwise
*/
sfColor dark_soften(app_t const *app, sfColor color)
{
    if (!app->lib->settings.dark)
        return color;
    color.r = color.r * 4 / 5;
    color.g = color.g * 4 / 5;
    color.b = color.b * 4 / 5;
    return color;
}

/*
** the picture of path with its colors inverted (r, g, b become 255 minus
** them, the transparency is kept): a dark picture becomes light
*/
sfTexture *dark_invert(char const *path)
{
    sfImage *image = sfImage_createFromFile(path);
    sfTexture *texture = NULL;
    sfVector2u size;
    sfColor pixel;

    if (!image)
        return NULL;
    size = sfImage_getSize(image);
    for (unsigned int y = 0; y < size.y; y++)
        for (unsigned int x = 0; x < size.x; x++) {
            pixel = sfImage_getPixel(image, x, y);
            pixel.r = 255 - pixel.r;
            pixel.g = 255 - pixel.g;
            pixel.b = 255 - pixel.b;
            sfImage_setPixel(image, x, y, pixel);
        }
    texture = sfTexture_createFromImage(image, NULL);
    sfImage_destroy(image);
    return texture;
}

/*
** the color given to the images and sprites (covers, icons of the
** tables): white keeps them as they are
*/
sfColor dark_tint(app_t const *app)
{
    if (app->lib->settings.dark)
        return (sfColor){205, 205, 205, 255};
    return sfWhite;
}
