/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** suggestions that credit an artist: "(X Remix)" gives the remixer,
** and a featuring written without brackets ("Title feat. X", "Title
** ft. X") gives the featured artist; the title is left as it is
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
    int where[2] = {group[0], group[1] - group[0] + 1};

    snprintf(remixer, NAME_LEN, "%.*s", len, name);
    if (len <= 0 || sug_style_name(remixer))
        return;
    sug_credit(ctx, where, remixer, ROLE_REMIX);
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
    char const *name = NULL;
    char artist[NAME_LEN];
    int where[2] = {0, 0};
    int len = 0;

    for (int i = 0; marks[i] && !found; i++)
        found = strstr(title, marks[i]);
    if (!found)
        return;
    name = strchr(found + 1, ' ') + 1;
    len = name_end(name);
    if (len <= 0 || len >= NAME_LEN)
        return;
    where[0] = found - title;
    where[1] = name + len - found;
    snprintf(artist, NAME_LEN, "%.*s", len, name);
    sug_credit(ctx, where, artist, ROLE_FEAT);
}
