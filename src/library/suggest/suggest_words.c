/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** what the title suggestions know for sure is not part of a title: the
** words of a video ("official video", "lyrics", "HD"...), the licenses
** (CC-BY, no copyright...) and the names of the artist or the uploader
*/

#include "mp3player.h"
#include <strings.h>

static char const *const noise[] = {"official", "officiel", "officielle",
    "video", "vidéo", "audio", "lyrics", "lyric", "paroles", "clip", "hd",
    "hq", "4k", "1080p", "720p", "60fps", "visualizer", "visualiser", "mv",
    "music", "musical", "remastered", "remaster", NULL};

static char const *const licenses[] = {"cc-by-sa", "cc by-sa", "cc-by-nc",
    "cc-by", "cc by", "cc0", "creative commons", "no copyright music",
    "no copyright", "copyright free", "royalty free", "free download",
    "ncs release", "nocopyrightsounds", NULL};

static char const *const styles[] = {"dubstep", "synthwave", "trap",
    "house", "techno", "edm", "dnb", "drum", "lofi", "lo", "phonk",
    "orchestral", "piano", "8", "bit", "metal", "rock", "jazz", "remix",
    "radio", "edit", "club", "extended", "vip", "official", "remastered",
    "version", "bootleg", "hardstyle", "trance", "future", "bass", NULL};

/*
** 1 if one of the words of name (a remixer) is a style: "Dubstep Remix"
** names no remixer
*/
int sug_style_name(char const *name)
{
    char clean[NAME_LEN];
    char **words = NULL;
    int style = 0;

    dup_clean(name, clean, NAME_LEN);
    words = stwa_delimiter(clean, " ", 0, 0);
    for (int i = 0; words && words[i]; i++)
        for (int j = 0; styles[j]; j++)
            style = style || strcmp(words[i], styles[j]) == 0;
    str_array_free(words);
    return style;
}

/*
** word in lowercase (dup_clean)
*/
int sug_noise_word(char const *word)
{
    for (int i = 0; noise[i]; i++)
        if (strcmp(noise[i], word) == 0)
            return 1;
    return 0;
}

/*
** length of the license written at the start of text, 0 if none (the
** next character must not be a letter: "cc by" is not "cc byte")
*/
int sug_license_at(char const *text)
{
    size_t len = 0;
    char next = 0;

    for (int i = 0; licenses[i]; i++) {
        len = strlen(licenses[i]);
        if (strncasecmp(text, licenses[i], len) != 0)
            continue;
        next = text[len];
        if (!((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z')))
            return len;
    }
    return 0;
}

/*
** the tag of a video is often the channel: "Name - Topic", "NameVEVO"
*/
static int same_uploader(char const *tag, char const *name)
{
    char clean[NAME_LEN];
    size_t len = 0;

    snprintf(clean, sizeof(clean), "%s", tag);
    len = strlen(clean);
    if (len > 8 && strcasecmp(clean + len - 8, " - Topic") == 0)
        clean[len - 8] = '\0';
    len = strlen(clean);
    if (len > 4 && strcasecmp(clean + len - 4, "VEVO") == 0)
        clean[len - 4] = '\0';
    return strcasecmp(clean, name) == 0;
}

/*
** 1 if name is an artist of the song, its uploader, or (any_known) an
** artist known by the library
*/
int sug_song_name(suggest_ctx_t const *ctx, char const *name, int any_known)
{
    artist_t *artist = NULL;

    if (name[0] == '\0')
        return 0;
    if (ctx->song->src_artists && same_uploader(ctx->song->src_artists, name))
        return 1;
    for (link_t *link = ctx->lib->links; link; link = link->next) {
        if (link->song_id != ctx->song->id)
            continue;
        artist = artist_by_id(ctx->lib, link->artist_id);
        if (artist && strcasecmp(artist->name, name) == 0)
            return 1;
    }
    return any_known && artist_find(ctx->lib, name) != NULL;
}
