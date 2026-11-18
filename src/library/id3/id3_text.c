/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** converts an ID3 text frame (latin-1, utf-16 or utf-8) to utf-8
*/

#include "mp3player.h"

/*
** Values separated by '\0' (multiple artists in v2.4) become "; ",
** other control characters (tabs, newlines) become spaces
** so that they can never break the .tsv save files.
*/
static int put_code(char *out, unsigned int code)
{
    if (code == 0) {
        out[0] = ';';
        out[1] = ' ';
        return 2;
    }
    if (code < 0x20)
        code = ' ';
    return utf8_encode(out, code);
}

static unsigned int read_utf16(unsigned char const *d, long *i, long size,
    int big)
{
    unsigned int unit = big ? (d[*i] << 8) | d[*i + 1]
        : d[*i] | (d[*i + 1] << 8);
    unsigned int low = 0;

    *i += 2;
    if (unit < 0xD800 || unit > 0xDBFF || *i + 1 >= size)
        return unit;
    low = big ? (d[*i] << 8) | d[*i + 1] : d[*i] | (d[*i + 1] << 8);
    if (low < 0xDC00 || low > 0xDFFF)
        return '?';
    *i += 2;
    return 0x10000 + ((unit - 0xD800) << 10) + (low - 0xDC00);
}

static long decode_utf16(char *out, unsigned char const *d, long size,
    int big)
{
    long i = 0;
    long n = 0;

    if (size >= 2 && d[0] == 0xFF && d[1] == 0xFE) {
        big = 0;
        i = 2;
    } else if (size >= 2 && d[0] == 0xFE && d[1] == 0xFF) {
        big = 1;
        i = 2;
    }
    while (i + 1 < size)
        n += put_code(out + n, read_utf16(d, &i, size, big));
    return n;
}

static long decode_bytes(char *out, unsigned char const *d, long size,
    int latin)
{
    long n = 0;

    for (long i = 0; i < size; i++) {
        if (latin || d[i] < 0x20)
            n += put_code(out + n, d[i]);
        else {
            out[n] = d[i];
            n++;
        }
    }
    return n;
}

char *id3_decode_text(unsigned char const *data, long size)
{
    char *out = NULL;
    long n = 0;

    if (size < 1)
        return NULL;
    out = malloc(size * 4 + 1);
    if (!out)
        return NULL;
    if (data[0] == 1 || data[0] == 2)
        n = decode_utf16(out, data + 1, size - 1, data[0] == 2);
    else
        n = decode_bytes(out, data + 1, size - 1, data[0] == 0);
    while (n >= 2 && out[n - 2] == ';' && out[n - 1] == ' ')
        n -= 2;
    out[n] = '\0';
    return out;
}
