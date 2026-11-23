/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** videos that YouTube no longer serves because the song was uploaded
** again (dl_reason_lasting): kept in unavailable.tsv with the reason,
** left out of the next downloads until they are tried again
*/

#include "mp3player.h"

rejected_t *unavailable_find(library_t *lib, char const *id)
{
    for (rejected_t *un = lib->unavailable; un; un = un->next)
        if (strcmp(un->id, id) == 0)
            return un;
    return NULL;
}

/*
** added at the end of the list; an id already there stays as it is
*/
rejected_t *unavailable_add(library_t *lib, char const *id,
    char const *title, char const *reason)
{
    rejected_t **last = &lib->unavailable;
    rejected_t *un = unavailable_find(lib, id);

    if (un)
        return un;
    un = calloc(1, sizeof(rejected_t));
    if (!un)
        return NULL;
    snprintf(un->id, VIDEO_ID, "%s", id);
    un->title = strdup(title ? title : "");
    un->reason = strdup(reason ? reason : "");
    un->duration = -1;
    un->date = time(NULL);
    while (*last)
        last = &(*last)->next;
    *last = un;
    return un;
}

/*
** id = NULL frees all of them
*/
void unavailable_remove(library_t *lib, char const *id)
{
    rejected_t **cur = &lib->unavailable;
    rejected_t *old = NULL;

    while (*cur) {
        if (id && strcmp((*cur)->id, id) != 0) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old->title);
        free(old->reason);
        free(old);
    }
}

int unavailable_count(library_t const *lib)
{
    int nb = 0;

    for (rejected_t const *un = lib->unavailable; un; un = un->next)
        nb++;
    return nb;
}
