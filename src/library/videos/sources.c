/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** addresses of the playlists of a site linked to local playlists (one
** at most per local playlist): "Synchroniser" downloads them again
*/

#include "mp3player.h"

source_t *source_get(library_t *lib, int playlist_id)
{
    for (source_t *src = lib->sources; src; src = src->next)
        if (src->playlist_id == playlist_id)
            return src;
    return NULL;
}

static source_t *source_new(library_t *lib, int playlist_id, char *url)
{
    source_t **last = &lib->sources;
    source_t *src = calloc(1, sizeof(source_t));

    if (!src) {
        free(url);
        return NULL;
    }
    src->playlist_id = playlist_id;
    src->url = url;
    src->min_s = -1;
    src->max_s = -1;
    while (*last)
        last = &(*last)->next;
    *last = src;
    return src;
}

/*
** the link of the playlist is replaced if it had one; the options are
** set by the caller
*/
source_t *source_set(library_t *lib, int playlist_id, char const *url)
{
    source_t *src = source_get(lib, playlist_id);
    char *copy = strdup(url);

    if (!copy)
        return NULL;
    if (!src)
        return source_new(lib, playlist_id, copy);
    free(src->url);
    src->url = copy;
    return src;
}

/*
** playlist_id = -1 frees all of them
*/
void source_remove(library_t *lib, int playlist_id)
{
    source_t **cur = &lib->sources;
    source_t *old = NULL;

    while (*cur) {
        if (playlist_id != -1 && (*cur)->playlist_id != playlist_id) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old->url);
        free(old);
    }
}
