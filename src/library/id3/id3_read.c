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

static void parse_tag(song_t *song, unsigned char const *head,
    unsigned char *tag, long size)
{
    long skip = 0;

    if (head[5] & 0x40)
        skip = extended_header_size(tag, size, head[3]);
    if (skip < 0 || skip >= size)
        return;
    id3_parse_frames(song, tag + skip, size - skip, head[3]);
}

void id3_read(song_t *song, char const *path)
{
    FILE *file = fopen(path, "rb");
    unsigned char head[10];
    unsigned char *tag = NULL;
    long size = 0;

    if (!file)
        return;
    if (fread(head, 1, 10, file) == 10 && memcmp(head, "ID3", 3) == 0
        && (head[3] == 3 || head[3] == 4)) {
        size = id3_synchsafe(head + 6);
        tag = malloc(size);
    }
    if (tag && fread(tag, 1, size, file) == (size_t)size)
        parse_tag(song, head, tag, size);
    free(tag);
    fclose(file);
}
