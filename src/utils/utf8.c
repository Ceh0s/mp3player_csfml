/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** utf-8 encoding and decoding (for metadata and sfText)
*/

#include "mp3player.h"

int utf8_encode(char *out, unsigned int code)
{
    if (code < 0x80) {
        out[0] = code;
        return 1;
    }
    if (code < 0x800) {
        out[0] = 0xC0 | (code >> 6);
        out[1] = 0x80 | (code & 0x3F);
        return 2;
    }
    if (code < 0x10000) {
        out[0] = 0xE0 | (code >> 12);
        out[1] = 0x80 | ((code >> 6) & 0x3F);
        out[2] = 0x80 | (code & 0x3F);
        return 3;
    }
    out[0] = 0xF0 | (code >> 18);
    out[1] = 0x80 | ((code >> 12) & 0x3F);
    out[2] = 0x80 | ((code >> 6) & 0x3F);
    out[3] = 0x80 | (code & 0x3F);
    return 4;
}

static int sequence_length(unsigned char c)
{
    if (c < 0x80)
        return 1;
    if ((c & 0xE0) == 0xC0)
        return 2;
    if ((c & 0xF0) == 0xE0)
        return 3;
    if ((c & 0xF8) == 0xF0)
        return 4;
    return 0;
}

static int decode_one(unsigned char const *str, sfUint32 *code)
{
    int len = sequence_length(str[0]);
    unsigned char const masks[5] = {0, 0x7F, 0x1F, 0x0F, 0x07};

    if (len == 0) {
        *code = '?';
        return 1;
    }
    *code = str[0] & masks[len];
    for (int i = 1; i < len; i++) {
        if ((str[i] & 0xC0) != 0x80) {
            *code = '?';
            return i;
        }
        *code = (*code << 6) | (str[i] & 0x3F);
    }
    return len;
}

sfUint32 *utf8_to_utf32(char const *str)
{
    unsigned char const *s = (unsigned char const *)str;
    sfUint32 *out = malloc(sizeof(sfUint32) * (strlen(str) + 1));
    int n = 0;

    if (!out)
        return NULL;
    while (*s) {
        s += decode_one(s, &out[n]);
        n++;
    }
    out[n] = 0;
    return out;
}
