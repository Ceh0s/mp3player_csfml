/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** text tools of the title suggestions: a title made tidy after a cut
** (single spaces, no " -" left at the end, no empty brackets), and the
** suggestion added to the list
*/

#include "mp3player.h"

/*
** one space between the words, none at the start or the end
*/
static void single_spaces(char *title)
{
    int out = 0;

    for (int i = 0; title[i]; i++) {
        if (title[i] == ' ' && (out == 0 || title[out - 1] == ' '))
            continue;
        title[out] = title[i];
        out++;
    }
    while (out > 0 && title[out - 1] == ' ')
        out--;
    title[out] = '\0';
}

static int empty_brackets(char *title)
{
    char *found = strstr(title, "()");

    if (!found)
        found = strstr(title, "[]");
    if (!found)
        return 0;
    memmove(found, found + 2, strlen(found + 2) + 1);
    return 1;
}

void sug_tidy(char *title)
{
    int removed = empty_brackets(title);
    int len = 0;

    while (removed)
        removed = empty_brackets(title);
    single_spaces(title);
    len = strlen(title);
    while (len > 0 && strchr("-| ", title[len - 1]))
        len--;
    title[len] = '\0';
    while (title[0] != '\0' && strchr("-| ", title[0]))
        memmove(title, title + 1, strlen(title));
}

/*
** out = the title without its bytes start..end (excluded), made tidy
*/
void sug_cut(char const *title, int start, int end, char *out)
{
    snprintf(out, NAME_LEN, "%.*s%s", start, title, title + end);
    sug_tidy(out);
}

/*
** the suggestion is kept only if it changes the title, does not empty
** it and is not there yet; returns it (to give it an artist) or NULL
*/
suggest_t *sug_add(suggest_ctx_t *ctx, suggest_rule_t rule,
    char const *removed, char const *title)
{
    suggest_t *sug = NULL;

    if (ctx->out->nb >= SUGGEST_MAX || title[0] == '\0'
        || strcmp(title, ctx->title) == 0)
        return NULL;
    for (int i = 0; i < ctx->out->nb; i++)
        if (strcmp(ctx->out->items[i].removed, removed) == 0)
            return NULL;
    sug = &ctx->out->items[ctx->out->nb];
    memset(sug, 0, sizeof(suggest_t));
    sug->rule = rule;
    snprintf(sug->removed, NAME_LEN, "%s", removed);
    snprintf(sug->title, NAME_LEN, "%s", title);
    ctx->out->nb++;
    return sug;
}

/*
** what is taken out of ctx->title: len bytes from start
*/
suggest_t *sug_push(suggest_ctx_t *ctx, suggest_rule_t rule, int start,
    int len)
{
    char removed[NAME_LEN];
    char title[NAME_LEN];

    if (len <= 0 || len >= NAME_LEN)
        return NULL;
    snprintf(removed, NAME_LEN, "%.*s", len, ctx->title + start);
    sug_cut(ctx->title, start, start + len, title);
    return sug_add(ctx, rule, removed, title);
}
