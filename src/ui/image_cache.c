/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** textures of the images of the songs and artists, loaded the first
** time they are shown and freed when the scene changes (only those of
** the current screen are in memory)
*/

#include "mp3player.h"

static image_t *image_load(app_t *app, rank_t const *rank, int small)
{
    char path[PATH_LEN];
    image_t *image = calloc(1, sizeof(image_t));

    if (!image)
        return NULL;
    image->artist = rank->artist != NULL;
    image->id = rank_id(rank);
    image->small = small;
    image_path(path, rank, small);
    if (access(path, F_OK) == 0)
        image->texture = sfTexture_createFromFile(path, NULL);
    if (image->texture)
        sfTexture_setSmooth(image->texture, sfTrue);
    image->next = app->images;
    app->images = image;
    return image;
}

/*
** small : 1 for the 120 px version; the default image (a note) is
** returned when the song or artist has none
*/
sfTexture *image_get(app_t *app, rank_t const *rank, int small)
{
    image_t *image = app->images;
    int artist = rank->artist != NULL;
    int id = rank_id(rank);

    while (image && (image->artist != artist || image->id != id
            || image->small != small))
        image = image->next;
    if (!image)
        image = image_load(app, rank, small);
    if (image && image->texture)
        return image->texture;
    if (app->lib->settings.dark)
        return app->no_cover_dark;
    return app->no_cover;
}

void image_cache_clear(app_t *app)
{
    image_t *next = NULL;

    while (app->images) {
        next = app->images->next;
        if (app->images->texture)
            sfTexture_destroy(app->images->texture);
        free(app->images);
        app->images = next;
    }
}
