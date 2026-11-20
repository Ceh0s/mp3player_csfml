/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** square thumbnails of an image, like the covers of YT Music: the
** centered square is kept, then scaled down to 400 px (big, battles,
** pages) and to 120 px (small, lists), so the screens never load a
** full size picture
*/

#include "mp3player.h"

/*
** the sprite shows the centered square of the texture, size px wide
*/
static void set_square(sfSprite *sprite, sfTexture *texture,
    unsigned int size)
{
    sfVector2u full = sfTexture_getSize(texture);
    int side = full.x < full.y ? full.x : full.y;
    sfIntRect crop = {(full.x - side) / 2, (full.y - side) / 2, side, side};
    float scale = (float)size / side;

    sfSprite_setTexture(sprite, texture, sfTrue);
    sfSprite_setTextureRect(sprite, crop);
    sfSprite_setScale(sprite, (sfVector2f){scale, scale});
}

/*
** the mipmaps keep a big picture smooth once scaled down
*/
static sfImage *render(sfTexture *texture, sfRenderTexture *target,
    sfSprite *sprite, unsigned int size)
{
    sfTexture_setSmooth(texture, sfTrue);
    sfTexture_generateMipmap(texture);
    set_square(sprite, texture, size);
    sfRenderTexture_clear(target, sfBlack);
    sfRenderTexture_drawSprite(target, sprite, NULL);
    sfRenderTexture_display(target);
    return sfTexture_copyToImage(sfRenderTexture_getTexture(target));
}

static sfImage *square(sfImage *source, unsigned int size)
{
    sfTexture *texture = sfTexture_createFromImage(source, NULL);
    sfRenderTexture *target = sfRenderTexture_create(size, size, sfFalse);
    sfSprite *sprite = sfSprite_create();
    sfImage *image = NULL;

    if (texture && target && sprite)
        image = render(texture, target, sprite, size);
    if (sprite)
        sfSprite_destroy(sprite);
    if (target)
        sfRenderTexture_destroy(target);
    if (texture)
        sfTexture_destroy(texture);
    return image;
}

/*
** the small one is made from the big one; returns 84 on error
*/
int thumbnail_save(sfImage *source, char const *big, char const *small)
{
    sfImage *large = square(source, IMAGE_BIG);
    sfImage *little = large ? square(large, IMAGE_SMALL) : NULL;
    int ret = 84;

    if (large && little && sfImage_saveToFile(large, big)
        && sfImage_saveToFile(little, small))
        ret = 0;
    if (large)
        sfImage_destroy(large);
    if (little)
        sfImage_destroy(little);
    return ret;
}
