/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** videos refused in the download screen because of their duration:
** they are left out again only while they stay out of the allowed
** duration (a wider one lets them be downloaded)
*/

#include "mp3player.h"

rejected_t *rejected_find(library_t *lib, char const *id)
{
    for (rejected_t *rej = lib->rejected; rej; rej = rej->next)
        if (strcmp(rej->id, id) == 0)
            return rej;
    return NULL;
}

/*
** added at the end of the list, refused now; an id already there is
** only updated
*/
rejected_t *rejected_add(library_t *lib, char const *id, char const *title,
    int duration)
{
    rejected_t **last = &lib->rejected;
    rejected_t *rej = rejected_find(lib, id);

    if (rej) {
        rej->duration = duration;
        return rej;
    }
    rej = calloc(1, sizeof(rejected_t));
    if (!rej)
        return NULL;
    snprintf(rej->id, VIDEO_ID, "%s", id);
    rej->title = strdup(title ? title : "");
    rej->duration = duration;
    rej->date = time(NULL);
    while (*last)
        last = &(*last)->next;
    *last = rej;
    return rej;
}

/*
** id = NULL frees all of them
*/
void rejected_remove(library_t *lib, char const *id)
{
    rejected_t **cur = &lib->rejected;
    rejected_t *old = NULL;

    while (*cur) {
        if (id && strcmp((*cur)->id, id) != 0) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old->title);
        free(old);
    }
}

int rejected_count(library_t const *lib)
{
    int nb = 0;

    for (rejected_t const *rej = lib->rejected; rej; rej = rej->next)
        nb++;
    return nb;
}
