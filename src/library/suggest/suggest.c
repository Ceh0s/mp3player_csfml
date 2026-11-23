/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** suggestions to clean the title of a song: only what is surely not
** part of the title (see suggest_words.c); each one changes one thing,
** so that it can be applied, hidden or deleted on its own. Albums,
** series or soundtracks in a title are left alone: no way to be sure.
*/

#include "mp3player.h"

static char const *const rule_names[NB_SUG_RULES] = {"spaces", "quotes",
    "hashtags", "prod", "feat", "remix", "prefix", "suffix", "noise",
    "license"};

char const *suggest_rule_name(int rule)
{
    return rule >= 0 && rule < NB_SUG_RULES ? rule_names[rule] : "?";
}

int suggest_rule_find(char const *name)
{
    for (int i = 0; i < NB_SUG_RULES; i++)
        if (strcmp(rule_names[i], name) == 0)
            return i;
    return -1;
}

static void spaces(suggest_ctx_t *ctx)
{
    char title[NAME_LEN];

    snprintf(title, NAME_LEN, "%s", ctx->title);
    sug_tidy(title);
    sug_add(ctx, SUG_SPACES, "  ", title);
}

/*
** "'Title' ..." or "\"Title\" ...": the quotes around the start go
*/
static void quotes(suggest_ctx_t *ctx)
{
    char const *title = ctx->title;
    char const *close = NULL;
    char removed[4] = {title[0], title[0], '\0', '\0'};
    char out[NAME_LEN];

    if (title[0] != '\'' && title[0] != '"')
        return;
    close = strchr(title + 1, title[0]);
    if (!close || close == title + 1 || (close[1] != ' ' && close[1] != '\0'))
        return;
    snprintf(out, NAME_LEN, "%.*s%s", (int)(close - title - 1), title + 1,
        close + 1);
    sug_tidy(out);
    sug_add(ctx, SUG_QUOTES, removed, out);
}

/*
** out gets the suggestions for title (the title being edited, or the
** one of the song)
*/
void suggest_list(library_t *lib, song_t *song, char const *title,
    suggest_list_t *out)
{
    suggest_ctx_t ctx = {lib, song, title, out};

    out->nb = 0;
    if (!title || title[0] == '\0')
        return;
    spaces(&ctx);
    quotes(&ctx);
    sug_hashtags(&ctx);
    sug_prod(&ctx);
    sug_groups(&ctx);
    sug_bare_feat(&ctx);
    sug_prefix(&ctx);
    sug_suffix(&ctx);
}
