/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** pairs of songs marked "Pas un doublon": never proposed again. Raw ids
** are kept, compared once resolved (as the playlists do).
*/

#include "mp3player.h"

/*
** a and b are resolved songs
*/
int not_dup_find(library_t *lib, song_t const *a, song_t const *b)
{
    song_t *first = NULL;
    song_t *second = NULL;

    for (not_dup_t *nd = lib->not_dups; nd; nd = nd->next) {
        first = song_resolve(lib, song_by_id(lib, nd->a));
        second = song_resolve(lib, song_by_id(lib, nd->b));
        if ((first == a && second == b) || (first == b && second == a))
            return 1;
    }
    return 0;
}

int not_dup_add(library_t *lib, int a, int b)
{
    not_dup_t *nd = calloc(1, sizeof(not_dup_t));

    if (!nd)
        return 84;
    nd->a = a;
    nd->b = b;
    nd->next = lib->not_dups;
    lib->not_dups = nd;
    return 0;
}

void not_dups_free(library_t *lib)
{
    not_dup_t *next = NULL;

    while (lib->not_dups) {
        next = lib->not_dups->next;
        free(lib->not_dups);
        lib->not_dups = next;
    }
}
