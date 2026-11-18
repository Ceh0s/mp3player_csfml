/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the known fingerprints (one per audio file ever seen)
*/

#include "mp3player.h"

fprint_t *fprint_find(library_t *lib, char const *hash)
{
    for (fprint_t *fp = lib->fprints; hash && fp; fp = fp->next)
        if (strcmp(fp->hash, hash) == 0)
            return fp;
    return NULL;
}

fprint_t *fprint_add(library_t *lib, char const *hash, int song_id)
{
    fprint_t *fp = calloc(1, sizeof(fprint_t));
    fprint_t *last = lib->fprints;

    if (!fp)
        return NULL;
    fp->hash = strdup(hash);
    fp->song_id = song_id;
    if (!last) {
        lib->fprints = fp;
        return fp;
    }
    while (last->next)
        last = last->next;
    last->next = fp;
    return fp;
}
