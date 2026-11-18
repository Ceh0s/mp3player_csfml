/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** builds the old (v1) key of a song: "main artist|title" in lowercase,
** only used to recognise the songs saved in the v1 format
*/

#include "mp3player.h"

static int is_space(char c)
{
    return c == ' ' || c == '\t';
}

static char lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');
    return c;
}

/*
** lowercase, trimmed, with repeated spaces collapsed
*/
static void append_clean(char *out, char const *str, char const *stop)
{
    int n = strlen(out);
    int space = 1;

    for (int i = 0; str[i] && !strchr(stop, str[i]); i++) {
        if (is_space(str[i]) && !space) {
            out[n] = ' ';
            n++;
        }
        space = is_space(str[i]);
        if (space)
            continue;
        out[n] = lower(str[i]);
        n++;
    }
    if (n > 0 && out[n - 1] == ' ')
        n--;
    out[n] = '\0';
}

char *song_make_key(song_t const *song)
{
    char *key = NULL;
    size_t len = 8;

    len += song->src_artists ? strlen(song->src_artists) : 0;
    len += song->src_title ? strlen(song->src_title) : 0;
    len += song->file ? strlen(song->file) : 0;
    key = calloc(len, sizeof(char));
    if (!key)
        return NULL;
    if (song->src_title && song->src_artists) {
        append_clean(key, song->src_artists, ";/");
        strcat(key, "|");
        append_clean(key, song->src_title, "");
    } else {
        strcpy(key, "file:");
        append_clean(key, song->file ? song->file : "?", "");
    }
    return key;
}
