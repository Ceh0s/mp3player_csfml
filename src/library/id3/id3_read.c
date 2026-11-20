/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads the ID3v2 tag at the start of an mp3 file
*/

#include "mp3player.h"

static long be32(unsigned char const *b)
{
    return ((long)b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
}

static long extended_header_size(unsigned char const *tag, long size,
    int version)
{
    if (size < 4)
        return size;
    if (version == 4)
        return id3_synchsafe(tag);
    return 4 + be32(tag);
}

static unsigned char *read_body(FILE *file, unsigned char const *head,
    long *size)
{
    unsigned char *data = NULL;
    size_t len = 0;

    if (memcmp(head, "ID3", 3) != 0 || (head[3] != 3 && head[3] != 4))
        return NULL;
    *size = id3_synchsafe(head + 6);
    len = *size;
    data = malloc(len);
    if (data && fread(data, 1, len, file) != len) {
        free(data);
        return NULL;
    }
    return data;
}

/*
** fills tag (tag->data is to free); returns 84 if the file has no
** readable ID3v2.3 / v2.4 tag
*/
int id3_load(char const *path, id3_tag_t *tag)
{
    FILE *file = fopen(path, "rb");
    unsigned char head[10] = {0};

    tag->data = NULL;
    if (!file)
        return 84;
    if (fread(head, 1, 10, file) == 10)
        tag->data = read_body(file, head, &tag->size);
    fclose(file);
    tag->version = head[3];
    tag->start = 0;
    if (tag->data && (head[5] & 0x40))
        tag->start = extended_header_size(tag->data, tag->size, head[3]);
    return tag->data ? 0 : 84;
}

void id3_read(song_t *song, char const *path)
{
    id3_tag_t tag;

    if (id3_load(path, &tag) == 84)
        return;
    if (tag.start >= 0 && tag.start < tag.size)
        id3_parse_frames(song, tag.data + tag.start, tag.size - tag.start,
            tag.version);
    free(tag.data);
}
