/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** suggestions on the ends of a title: "Artist - Title" and "Title -
** Artist" (the artist of the song, its uploader or a known artist),
** hashtags at the end (#shorts...), "Prod by X" at the end
*/

#include "mp3player.h"
#include <strings.h>

/*
** "X - Title": X becomes the artist (see suggest_apply.c)
*/
void sug_prefix(suggest_ctx_t *ctx)
{
    char const *dash = strstr(ctx->title, " - ");
    char name[NAME_LEN];
    suggest_t *sug = NULL;
    int len = dash ? dash - ctx->title : 0;

    if (!dash || len >= NAME_LEN)
        return;
    snprintf(name, len + 1, "%s", ctx->title);
    if (!sug_song_name(ctx, name, 1))
        return;
    sug = sug_push(ctx, SUG_PREFIX, 0, len + 3);
    if (!sug)
        return;
    snprintf(sug->artist, NAME_LEN, "%s", name);
    sug->role = ROLE_MAIN;
}

/*
** "X - Y" where neither X nor Y is a known name (a video uploaded by
** someone else, "32 Leaves - Waiting"): both ways are proposed, as
** guesses (never applied by "Tout appliquer"); not for the songs made
** by YouTube itself, whose titles are right
*/
void sug_guess(suggest_ctx_t *ctx)
{
    char const *dash = strstr(ctx->title, " - ");
    int len = dash ? dash - ctx->title : 0;
    suggest_t *sug = NULL;

    for (int i = 0; i < ctx->out->nb; i++)
        if (ctx->out->items[i].rule == SUG_PREFIX
            || ctx->out->items[i].rule == SUG_SUFFIX)
            return;
    if (!dash || len == 0 || len >= NAME_LEN || ctx->song->autogen
        || strstr(dash + 3, " - ") || dash[3] == '\0')
        return;
    sug = sug_push(ctx, SUG_ARTIST_FIRST, 0, len + 3);
    if (sug) {
        snprintf(sug->artist, NAME_LEN, "%.*s", len, ctx->title);
        sug->role = ROLE_MAIN;
    }
    sug = sug_push(ctx, SUG_TITLE_FIRST, len, strlen(dash));
    if (sug) {
        snprintf(sug->artist, NAME_LEN, "%s", dash + 3);
        sug->role = ROLE_MAIN;
    }
}

/*
** "Title - X", the last " - " of the title
*/
void sug_suffix(suggest_ctx_t *ctx)
{
    char const *dash = NULL;
    suggest_t *sug = NULL;
    char const *found = strstr(ctx->title, " - ");

    while (found) {
        dash = found;
        found = strstr(found + 1, " - ");
    }
    if (!dash || dash == ctx->title || !sug_song_name(ctx, dash + 3, 1))
        return;
    sug = sug_push(ctx, SUG_SUFFIX, dash - ctx->title, strlen(dash));
    if (!sug)
        return;
    snprintf(sug->artist, NAME_LEN, "%s", dash + 3);
    sug->role = ROLE_MAIN;
}

/*
** every "#word" at the end of the title
*/
void sug_hashtags(suggest_ctx_t *ctx)
{
    char const *title = ctx->title;
    int end = strlen(title);
    int start = end;
    int word = end;

    while (word > 0) {
        while (word > 0 && title[word - 1] == ' ')
            word--;
        while (word > 0 && title[word - 1] != ' ')
            word--;
        if (title[word] != '#')
            break;
        start = word;
    }
    if (start < end)
        sug_push(ctx, SUG_HASHTAGS, start, end - start);
}

/*
** " Prod by X", " Prod. X" up to the end of the title
*/
void sug_prod(suggest_ctx_t *ctx)
{
    char const *title = ctx->title;

    for (int i = 0; title[i]; i++) {
        if (title[i] != ' ' || strncasecmp(title + i + 1, "prod", 4) != 0)
            continue;
        if (strncasecmp(title + i + 5, " by ", 4) == 0
            || title[i + 5] == '.') {
            sug_push(ctx, SUG_PROD, i, strlen(title + i));
            return;
        }
    }
}
