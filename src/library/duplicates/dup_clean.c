/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** texts compared to find duplicates: lowercase words without
** punctuation, the core of a title (without what changes from an upload
** to another: "Artist - ", brackets, featurings, "official video"...)
** and its version words (a remix is not the original)
*/

#include "mp3player.h"
#include <ctype.h>

static char const *const versions[] = {"remix", "live", "acoustic",
    "sped", "slowed", "nightcore", "extended", "instrumental", "vip",
    "radio", "cover", "reverb", "mashup", "bootleg", "karaoke", "remake",
    "reversed", NULL};

static char const *const noise[] = {"official", "officiel", "video",
    "audio", "lyrics", "lyric", "clip", "hd", "hq", "4k", "visualizer",
    "visualiser", "mv", "remastered", "remaster", NULL};

/*
** ascii letters in lowercase, other ascii characters become one space,
** utf-8 characters (accents, japanese...) are kept as they are
*/
void dup_clean(char const *in, char *out, int size)
{
    int n = 0;
    int space = 1;
    unsigned char c = 0;

    for (int i = 0; in[i] && n < size - 1; i++) {
        c = (unsigned char)in[i];
        if (c < 128 && !isalnum(c)) {
            out[n] = ' ';
            n += !space;
            space = 1;
            continue;
        }
        out[n] = tolower(c);
        n++;
        space = 0;
    }
    n -= n > 0 && out[n - 1] == ' ';
    out[n] = '\0';
}

/*
** 1 if word is one of the words of the clean text
*/
int dup_has_word(char const *clean, char const *word)
{
    size_t len = strlen(word);

    for (char const *found = strstr(clean, word); found;
        found = strstr(found + 1, word))
        if ((found == clean || found[-1] == ' ')
            && (found[len] == ' ' || found[len] == '\0'))
            return 1;
    return 0;
}

int dup_version(char const *clean)
{
    int bits = 0;

    for (int i = 0; versions[i]; i++)
        if (dup_has_word(clean, versions[i]))
            bits |= 1 << i;
    return bits;
}

/*
** the noise words are removed (unless nothing would be left)
*/
void dup_remove_noise(char *clean)
{
    char words[NAME_LEN];
    char out[NAME_LEN] = "";
    char *save = NULL;
    int keep = 0;

    snprintf(words, NAME_LEN, "%s", clean);
    for (char *word = strtok_r(words, " ", &save); word;
        word = strtok_r(NULL, " ", &save)) {
        keep = 1;
        for (int i = 0; noise[i] && keep; i++)
            keep = strcmp(word, noise[i]) != 0;
        if (keep && strlen(out) + strlen(word) + 2 < NAME_LEN) {
            strcat(out, out[0] ? " " : "");
            strcat(out, word);
        }
    }
    if (out[0])
        strcpy(clean, out);
}
