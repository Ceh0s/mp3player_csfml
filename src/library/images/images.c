/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** images of the songs and artists, in data/images/{songs,artists}/:
** <id>.jpg (big) and <id>_s.jpg (small). A song gets the cover of its
** mp3 by default, any image file can replace it (the mp3 is never
** modified, the image is copied there)
*/

#include "mp3player.h"
#include <sys/stat.h>

/*
** buf holds PATH_LEN bytes
*/
void image_path(char *buf, rank_t const *rank, int small)
{
    snprintf(buf, PATH_LEN, "%s/%s/%d%s.jpg", IMAGES_DIR,
        rank->song ? "songs" : "artists", rank_id(rank), small ? "_s" : "");
}

static int image_store(sfImage *image, rank_t const *rank)
{
    char big[PATH_LEN];
    char small[PATH_LEN];

    mkdir(DATA_DIR, 0755);
    mkdir(IMAGES_DIR, 0755);
    mkdir(IMAGES_DIR "/songs", 0755);
    mkdir(IMAGES_DIR "/artists", 0755);
    image_path(big, rank, 0);
    image_path(small, rank, 1);
    return thumbnail_save(image, big, small);
}

/*
** file is a path typed or pasted in a field; returns 84 if it is not a
** readable image
*/
int image_import(rank_t const *rank, char const *file)
{
    char path[NAME_LEN];
    sfImage *image = NULL;
    int ret = 84;

    path_clean(file, path);
    image = sfImage_createFromFile(path);
    if (image) {
        ret = image_store(image, rank);
        sfImage_destroy(image);
    }
    return ret;
}

/*
** rank->song must be a present song; returns 84 if it has no cover
*/
int image_from_cover(rank_t const *rank)
{
    long size = 0;
    unsigned char *bytes = id3_cover(rank->song->path, &size);
    sfImage *image = bytes ? sfImage_createFromMemory(bytes, size) : NULL;
    int ret = 84;

    if (image) {
        ret = image_store(image, rank);
        sfImage_destroy(image);
    }
    free(bytes);
    return ret;
}

/*
** at load: the present songs without an image get the cover of their
** mp3 (done once: the image file then exists)
*/
void images_from_covers(library_t *lib)
{
    char path[PATH_LEN];
    rank_t rank = {NULL, NULL, 0, 0, 0};

    for (int i = 0; i < lib->nb; i++) {
        rank.song = lib->present[i];
        image_path(path, &rank, 0);
        if (access(path, F_OK) != 0)
            image_from_cover(&rank);
    }
}
