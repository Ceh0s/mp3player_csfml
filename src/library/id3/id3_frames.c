/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** walks through the frames of an ID3v2.3 / v2.4 tag
*/

#include "mp3player.h"

/*
** v2.3 frame sizes are plain big endian, v2.4 ones are synchsafe
*/
long id3_frame_size(unsigned char const *b, int version)
{
    if (version == 4)
        return id3_synchsafe(b);
    return ((long)b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
}

static char **frame_target(song_t *song, char const *id)
{
    if (strncmp(id, "TIT2", 4) == 0)
        return &song->src_title;
    if (strncmp(id, "TPE1", 4) == 0)
        return &song->src_artists;
    if (strncmp(id, "TALB", 4) == 0)
        return &song->album;
    if (strncmp(id, "TCON", 4) == 0)
        return &song->genre;
    if (strncmp(id, "TYER", 4) == 0 || strncmp(id, "TDRC", 4) == 0)
        return &song->year;
    return NULL;
}

/*
** a TXXX frame is a description and a value, separated by '\0' (which
** id3_decode_text turns into "; "): yt-dlp writes "purl" = the address
** of the video
*/
static void store_user_text(song_t *song, unsigned char const *data,
    long size)
{
    char *text = id3_decode_text(data, size);

    if (text && !song->source && strncmp(text, "purl; ", 6) == 0
        && text[6] != '\0')
        song->source = strdup(text + 6);
    free(text);
}

static void store_length(song_t *song, unsigned char const *data,
    long size)
{
    char *text = id3_decode_text(data, size);

    song->duration_ms = text ? atol(text) : 0;
    free(text);
}

static void store_frame(song_t *song, char const *id,
    unsigned char const *data, long size)
{
    char **target = frame_target(song, id);
    char *text = NULL;

    if (strncmp(id, "TXXX", 4) == 0)
        store_user_text(song, data, size);
    if (strncmp(id, "TLEN", 4) == 0)
        store_length(song, data, size);
    if (!target || *target)
        return;
    text = id3_decode_text(data, size);
    if (text && text[0] == '\0') {
        free(text);
        text = NULL;
    }
    *target = text;
}

void id3_parse_frames(song_t *song, unsigned char const *tag, long size,
    int version)
{
    long pos = 0;
    long fsize = 0;

    while (pos + 10 <= size && tag[pos] != '\0') {
        fsize = id3_frame_size(tag + pos + 4, version);
        if (fsize <= 0 || pos + 10 + fsize > size)
            return;
        store_frame(song, (char const *)tag + pos, tag + pos + 10, fsize);
        pos += 10 + fsize;
    }
}
