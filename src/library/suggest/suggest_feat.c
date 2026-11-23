/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** suggestions that add an artist: "(X Remix)" gives the remixer, and a
** featuring written without brackets ("Title feat. X", "Title ft. X")
** gives the featured artist
*/

#include "mp3player.h"
#include <strings.h>

/*
** group = {'(' , ')'}; name holds the text in the brackets, the remixer
** is its len first bytes; nothing if it is a style ("Dubstep Remix")
*/
void sug_remix(suggest_ctx_t *ctx, int const *group, char const *name,
    int len)
{
    char remixer[NAME_LEN];
    suggest_t *sug = NULL;

    snprintf(remixer, NAME_LEN, "%.*s", len, name);
    if (len <= 0 || sug_style_name(remixer))
        return;
    sug = sug_push(ctx, SUG_REMIX, group[0], group[1] - group[0] + 1);
    if (!sug)
        return;
    snprintf(sug->artist, NAME_LEN, "%.*s", len, name);
    sug->role = ROLE_REMIX;
}

/*
** where the name of the featured artist ends: before brackets, before
** " - ", or at the end of the title
*/
static int name_end(char const *name)
{
    int len = 0;

    while (name[len] && name[len] != '(' && name[len] != '['
        && strncmp(name + len, " - ", 3) != 0)
        len++;
    while (len > 0 && name[len - 1] == ' ')
        len--;
    return len;
}

void sug_bare_feat(suggest_ctx_t *ctx)
{
    char const *title = ctx->title;
    char const *const marks[] = {" feat. ", " ft. ", " Feat. ", " Ft. ",
        NULL};
    char const *found = NULL;
    suggest_t *sug = NULL;
    int len = 0;

    for (int i = 0; marks[i] && !found; i++)
        found = strstr(title, marks[i]);
    if (!found)
        return;
    len = name_end(strchr(found + 1, ' ') + 1);
    sug = sug_push(ctx, SUG_FEAT, found - title,
        strchr(found + 1, ' ') + 1 + len - found);
    if (!sug || len <= 0)
        return;
    snprintf(sug->artist, NAME_LEN, "%.*s", len, strchr(found + 1, ' ') + 1);
    sug->role = ROLE_FEAT;
}
