/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** checks that a file really is an mp3 by reading its first bytes
** (the extension alone proves nothing: music/text.mp3 is plain text)
*/

#include "mp3player.h"

/*
** ID3 sizes are "synchsafe": 4 bytes of which only the 7 low bits count,
** so that the size can never be mistaken for an mp3 frame header (0xFF..).
*/
long id3_synchsafe(unsigned char const *bytes)
{
    return ((long)(bytes[0] & 0x7F) << 21) | ((bytes[1] & 0x7F) << 14)
        | ((bytes[2] & 0x7F) << 7) | (bytes[3] & 0x7F);
}

/*
** An mp3 frame header starts with 11 bits set (0xFFE),
** followed by version, layer, bitrate and samplerate fields
** which have some forbidden values.
*/
static int is_frame_header(unsigned char const *b)
{
    if (b[0] != 0xFF || (b[1] & 0xE0) != 0xE0)
        return 0;
    if (((b[1] >> 3) & 3) == 1 || ((b[1] >> 1) & 3) == 0)
        return 0;
    if ((b[2] >> 4) == 15 || ((b[2] >> 2) & 3) == 3)
        return 0;
    return 1;
}

long mp3_audio_offset(unsigned char const *head)
{
    long offset = 0;

    if (memcmp(head, "ID3", 3) == 0) {
        offset = 10 + id3_synchsafe(head + 6);
        if (head[5] & 0x10)
            offset += 10;
    }
    return offset;
}

static int find_frame(FILE *file, long offset)
{
    unsigned char buf[4096];
    size_t len = 0;

    if (fseek(file, offset, SEEK_SET) != 0)
        return 0;
    len = fread(buf, 1, sizeof(buf), file);
    for (size_t i = 0; i + 3 < len; i++)
        if (is_frame_header(buf + i))
            return 1;
    return 0;
}

int mp3_is_valid(char const *path)
{
    FILE *file = fopen(path, "rb");
    unsigned char head[10];
    int valid = 0;

    if (!file)
        return 0;
    if (fread(head, 1, 10, file) == 10)
        valid = find_frame(file, mp3_audio_offset(head));
    fclose(file);
    return valid;
}
