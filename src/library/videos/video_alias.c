/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** videos of a playlist of a site played under another id (no longer
** served: download_swap.c, download_reupload.c), kept in
** video_aliases.tsv: a Takeout file gives the id kept by the playlist,
** the file in music/ has the one of the video played
*/

#include "mp3player.h"

video_alias_t *video_alias_find(library_t *lib, char const *from)
{
    for (video_alias_t *alias = lib->aliases; alias; alias = alias->next)
        if (strcmp(alias->from, from) == 0)
            return alias;
    return NULL;
}

/*
** the last video found for an id replaces the one before
*/
void video_alias_add(library_t *lib, char const *from, char const *to)
{
    video_alias_t *alias = video_alias_find(lib, from);

    if (from[0] == '\0' || to[0] == '\0' || strcmp(from, to) == 0)
        return;
    if (!alias) {
        alias = calloc(1, sizeof(video_alias_t));
        if (!alias)
            return;
        snprintf(alias->from, VIDEO_ID, "%s", from);
        alias->next = lib->aliases;
        lib->aliases = alias;
    }
    snprintf(alias->to, VIDEO_ID, "%s", to);
}

void video_aliases_free(library_t *lib)
{
    video_alias_t *next = NULL;

    while (lib->aliases) {
        next = lib->aliases->next;
        free(lib->aliases);
        lib->aliases = next;
    }
}

void db_parse_alias(library_t *lib, char **fields, int nb)
{
    if (nb == 2)
        video_alias_add(lib, fields[0], fields[1]);
}

int db_save_aliases(library_t *lib)
{
    FILE *file = tsv_create(ALIASES_DB, ALIASES_COLUMNS);

    if (!file)
        return 84;
    for (video_alias_t *alias = lib->aliases; alias; alias = alias->next)
        fprintf(file, "%s\t%s\n", alias->from, alias->to);
    return tsv_commit(file, ALIASES_DB);
}
