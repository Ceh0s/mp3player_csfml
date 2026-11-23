/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** suggestions on the brackets of a title, "(...)" or "[...]": only
** noise words ("Official Video", "HD"), the name of the artist or of
** the uploader, a license (only the license goes if the brackets hold
** other words), a featuring or a remixer (the artist is added), a
** producer; and a featuring written without brackets
*/

#include "mp3player.h"
#include <strings.h>

/*
** every word of text is a noise word ("Official Music Video")
*/
static int all_noise(char const *text, int len)
{
    char raw[NAME_LEN];
    char clean[NAME_LEN];
    char **words = NULL;
    int noise = 1;

    snprintf(raw, NAME_LEN, "%.*s", len, text);
    dup_clean(raw, clean, NAME_LEN);
    words = stwa_delimiter(clean, " ", 0, 0);
    if (!words || !words[0])
        noise = 0;
    for (int i = 0; words && words[i]; i++)
        noise = noise && sug_noise_word(words[i]);
    str_array_free(words);
    return noise;
}

/*
** marker of an artist at the start of the brackets: "feat. ", "ft. "...
*/
static int marker(char const *text)
{
    char const *const marks[] = {"feat. ", "ft. ", "featuring ", "w/ ",
        NULL};

    for (int i = 0; marks[i]; i++)
        if (strncasecmp(text, marks[i], strlen(marks[i])) == 0)
            return strlen(marks[i]);
    return 0;
}

/*
** the brackets start..end (both included) go, artist (len bytes) is
** added with role
*/
static void with_artist(suggest_ctx_t *ctx, int const *group,
    char const *artist, int len)
{
    suggest_t *sug = sug_push(ctx, SUG_FEAT, group[0],
        group[1] - group[0] + 1);

    if (!sug || len <= 0 || len >= NAME_LEN)
        return;
    snprintf(sug->artist, NAME_LEN, "%.*s", len, artist);
    sug->role = ROLE_FEAT;
}

/*
** only the license goes (with the space before it): "[Cinematic CC-BY]"
** becomes "[Cinematic]"
*/
static void license(suggest_ctx_t *ctx, int const *group)
{
    char const *title = ctx->title;
    int len = 0;

    for (int i = group[0] + 1; i < group[1]; i++) {
        len = sug_license_at(title + i);
        if (len == 0 || (title[i - 1] != ' ' && i != group[0] + 1))
            continue;
        if (title[i - 1] == ' ')
            sug_push(ctx, SUG_LICENSE, i - 1, len + 1);
        else
            sug_push(ctx, SUG_LICENSE, i, len);
        return;
    }
}

/*
** group = {index of '(' or '[', index of the closing one}
*/
static void group(suggest_ctx_t *ctx, int const *group)
{
    char const *in = ctx->title + group[0] + 1;
    int len = group[1] - group[0] - 1;
    char name[NAME_LEN];

    snprintf(name, NAME_LEN, "%.*s", len, in);
    if (marker(in) > 0) {
        with_artist(ctx, group, in + marker(in), len - marker(in));
        return;
    }
    if (len > 6 && strncasecmp(in + len - 6, " remix", 6) == 0) {
        sug_remix(ctx, group, name, len - 6);
        return;
    }
    if (strncasecmp(in, "prod", 4) == 0 || all_noise(in, len)
        || sug_song_name(ctx, name, 0)) {
        sug_push(ctx, strncasecmp(in, "prod", 4) == 0 ? SUG_PROD
            : SUG_NOISE, group[0], len + 2);
        return;
    }
    license(ctx, group);
}

void sug_groups(suggest_ctx_t *ctx)
{
    char const *title = ctx->title;
    char const *close = NULL;
    int pos[2] = {0, 0};

    for (int i = 0; title[i]; i++) {
        if (title[i] != '(' && title[i] != '[')
            continue;
        close = strchr(title + i, title[i] == '(' ? ')' : ']');
        if (!close)
            return;
        pos[0] = i;
        pos[1] = close - title;
        group(ctx, pos);
        i = pos[1];
    }
}
