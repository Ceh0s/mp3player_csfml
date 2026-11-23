/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** repair of the song images shifted by one song (the image of a song
** was the cover of another one, seen on 2026-11-20): an image closer
** to the cover of another mp3 than to the cover of its own is made
** again from its own cover. An image imported by hand looks like no
** cover, so it is kept. Done once (COVER_CHECKED marks it).
*/

#include "mp3player.h"

/*
** adds the colours of the pixels x0..x1 of the row y (rgba pixels of
** an image width px wide) to sum
*/
static void add_row(sfUint8 const *pixels, unsigned int width,
    unsigned int const *row, long *sum)
{
    unsigned int pos = 0;

    for (unsigned int x = row[1]; x < row[2]; x++) {
        pos = (row[0] * width + x) * 4;
        sum[0] += pixels[pos];
        sum[1] += pixels[pos + 1];
        sum[2] += pixels[pos + 2];
        sum[3]++;
    }
}

/*
** average colour of the cell (cx, cy) of the centered square
*/
static void cell_average(sfImage *image, int cx, int cy, unsigned char *out)
{
    sfVector2u size = sfImage_getSize(image);
    unsigned int side = size.x < size.y ? size.x : size.y;
    unsigned int left = (size.x - side) / 2 + cx * side / COVER_SIG;
    unsigned int top = (size.y - side) / 2;
    long sum[4] = {0, 0, 0, 0};
    unsigned int row[3] = {0, left, left + side / COVER_SIG};

    for (unsigned int y = cy * side / COVER_SIG;
        y < (cy + 1) * side / COVER_SIG; y++) {
        row[0] = top + y;
        add_row(sfImage_getPixelsPtr(image), size.x, row, sum);
    }
    for (int i = 0; i < 3 && sum[3] > 0; i++)
        out[i] = sum[i] / sum[3];
}

/*
** frees image; returns 1 if there was one
*/
static int signature(sfImage *image, unsigned char *sig)
{
    if (!image)
        return 0;
    for (int cy = 0; cy < COVER_SIG; cy++)
        for (int cx = 0; cx < COVER_SIG; cx++)
            cell_average(image, cx, cy, sig + (cy * COVER_SIG + cx) * 3);
    sfImage_destroy(image);
    return 1;
}

/*
** mean difference of the colours (0 = the same image, 255 = opposite)
*/
static int distance(unsigned char const *a, unsigned char const *b)
{
    long sum = 0;

    for (int i = 0; i < COVER_SIG * COVER_SIG * 3; i++)
        sum += a[i] > b[i] ? a[i] - b[i] : b[i] - a[i];
    return sum / (COVER_SIG * COVER_SIG * 3);
}

static void read_song(song_t *song, cover_sig_t *sig)
{
    char path[PATH_LEN];
    rank_t rank = {song, NULL, 0, 0, 0};
    long size = 0;
    unsigned char *bytes = id3_cover(song->path, &size);

    sig->has_cover = signature(bytes ? sfImage_createFromMemory(bytes, size)
        : NULL, sig->cover);
    free(bytes);
    image_path(path, &rank, 0);
    sig->has_image = signature(sfImage_createFromFile(path), sig->image);
}

/*
** the image of song i is the cover of another song (and not its own)
*/
static int shifted(cover_sig_t const *sigs, int nb, int i)
{
    if (!sigs[i].has_image || !sigs[i].has_cover
        || distance(sigs[i].image, sigs[i].cover) <= COVER_SAME)
        return 0;
    for (int j = 0; j < nb; j++)
        if (j != i && sigs[j].has_cover
            && distance(sigs[i].image, sigs[j].cover) <= COVER_SAME)
            return 1;
    return 0;
}

void covers_repair(library_t *lib)
{
    cover_sig_t *sigs = NULL;
    rank_t rank = {NULL, NULL, 0, 0, 0};

    if (access(COVER_CHECKED, F_OK) == 0 || lib->nb == 0)
        return;
    sigs = calloc(lib->nb, sizeof(cover_sig_t));
    if (!sigs)
        return;
    for (int i = 0; i < lib->nb; i++)
        read_song(lib->present[i], &sigs[i]);
    for (int i = 0; i < lib->nb; i++) {
        rank.song = lib->present[i];
        if (shifted(sigs, lib->nb, i) && image_from_cover(&rank) == 0)
            fprintf(stderr, "Image repaired: %s\n", song_title(rank.song));
    }
    free(sigs);
    tsv_touch(COVER_CHECKED, "#checked");
}
