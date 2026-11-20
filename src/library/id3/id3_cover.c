/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** cover picture of an mp3 (first APIC frame of its ID3 tag), used as
** the default image of the song
*/

#include "mp3player.h"

/*
** APIC frame: encoding (1 byte), mime type ending by 0, picture type
** (1 byte), description ending by 0 (by 00 00 in utf-16), then the
** bytes of the image; returns where the image starts
*/
static long picture_start(unsigned char const *data, long size)
{
    long pos = 1;

    while (pos < size && data[pos] != 0)
        pos++;
    pos += 2;
    if (data[0] != 1 && data[0] != 2) {
        while (pos < size && data[pos] != 0)
            pos++;
        return pos + 1;
    }
    while (pos + 1 < size && (data[pos] != 0 || data[pos + 1] != 0))
        pos += 2;
    return pos + 2;
}

static unsigned char *copy_picture(unsigned char const *frame, long fsize,
    long *size)
{
    long start = picture_start(frame, fsize);
    unsigned char *picture = NULL;

    if (start >= fsize)
        return NULL;
    *size = fsize - start;
    picture = malloc(*size);
    if (picture)
        memcpy(picture, frame + start, *size);
    return picture;
}

static unsigned char *find_cover(id3_tag_t const *tag, long *size)
{
    unsigned char const *frames = tag->data + tag->start;
    long total = tag->size - tag->start;
    long pos = 0;
    long fsize = 0;

    while (pos + 10 <= total && frames[pos] != '\0') {
        fsize = id3_frame_size(frames + pos + 4, tag->version);
        if (fsize <= 0 || pos + 10 + fsize > total)
            return NULL;
        if (memcmp(frames + pos, "APIC", 4) == 0)
            return copy_picture(frames + pos + 10, fsize, size);
        pos += 10 + fsize;
    }
    return NULL;
}

/*
** returns the bytes of the picture (to free) and its size, NULL if the
** file has no cover
*/
unsigned char *id3_cover(char const *path, long *size)
{
    id3_tag_t tag;
    unsigned char *picture = NULL;

    if (!path || id3_load(path, &tag) == 84)
        return NULL;
    if (tag.start >= 0 && tag.start < tag.size)
        picture = find_cover(&tag, size);
    free(tag.data);
    return picture;
}
