/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** audio fingerprint of a file: hash of the start of its audio data,
** the tags are skipped so that renaming or re-tagging the file
** (in another software) does not change it
*/

#include "mp3player.h"

static unsigned long long fnv1a(unsigned char const *data, size_t len,
    unsigned long long hash)
{
    for (size_t i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

/*
** size of the audio data: file size minus the ID3v2 tag at the start
** and the 128 bytes ID3v1 tag ("TAG") that may be at the end
*/
static long audio_size(FILE *file, long offset)
{
    char tag[3];
    long size = 0;

    fseek(file, 0, SEEK_END);
    size = ftell(file) - offset;
    if (size > 128 && fseek(file, -128, SEEK_END) == 0
        && fread(tag, 1, 3, file) == 3 && memcmp(tag, "TAG", 3) == 0)
        size -= 128;
    return size;
}

static char *hash_file(FILE *file, unsigned char *buf)
{
    unsigned long long hash = 14695981039346656037ULL;
    long offset = 0;
    long size = 0;
    long to_read = 0;
    size_t len = 0;
    char *out = malloc(17);

    if (!out || fread(buf, 1, 10, file) != 10) {
        free(out);
        return NULL;
    }
    offset = mp3_audio_offset(buf);
    size = audio_size(file, offset);
    to_read = size < FPRINT_BYTES ? size : FPRINT_BYTES;
    fseek(file, offset, SEEK_SET);
    len = fread(buf, 1, to_read > 0 ? to_read : 0, file);
    hash = fnv1a(buf, len, hash);
    hash = fnv1a((unsigned char const *)&size, sizeof(size), hash);
    snprintf(out, 17, "%016llx", hash);
    return out;
}

char *fingerprint_file(char const *path)
{
    FILE *file = fopen(path, "rb");
    unsigned char *buf = malloc(FPRINT_BYTES);
    char *hash = NULL;

    if (file && buf)
        hash = hash_file(file, buf);
    if (file)
        fclose(file);
    free(buf);
    return hash;
}
