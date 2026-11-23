/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** sort chosen for each playlist (id 0 = "Tous les titres"): criterion,
** direction and custom order. Without a saved choice, a playlist keeps
** its own order and "Tous les titres" is sorted by title.
*/

#include "mp3player.h"

static const sort_def_t sort_defs[NB_SORTS] = {
    {"playlist", "Ajout playlist", 0},
    {"custom", "Personnalisé", 0},
    {"title", "Titre", 0},
    {"artist", "Artiste", 0},
    {"duration", "Durée", 0},
    {"listen", "Écoute", 1},
    {"added", "Ajout logiciel", 1},
    {"release", "Parution", 1}
};

sort_def_t const *sort_def(int key)
{
    if (key < 0 || key >= NB_SORTS)
        return &sort_defs[0];
    return &sort_defs[key];
}

/*
** returns -1 for an unknown name (a criterion removed since)
*/
int sort_key_by_name(char const *name)
{
    for (int i = 0; i < NB_SORTS; i++)
        if (strcmp(sort_defs[i].name, name) == 0)
            return i;
    return -1;
}

/*
** the sort of a playlist, created with the default one if needed
** (NULL only if the memory is full)
*/
view_sort_t *sort_get(library_t *lib, int playlist_id)
{
    view_sort_t *sort = lib->sorts;

    while (sort && sort->playlist_id != playlist_id)
        sort = sort->next;
    if (sort)
        return sort;
    sort = calloc(1, sizeof(view_sort_t));
    if (!sort)
        return NULL;
    sort->playlist_id = playlist_id;
    sort->key = playlist_id == 0 ? SORT_TITLE : SORT_PLAYLIST;
    sort->next = lib->sorts;
    lib->sorts = sort;
    return sort;
}

/*
** the criterion already chosen changes direction, another one is chosen
** in its usual direction; the custom order has only one direction
*/
void sort_set(library_t *lib, int playlist_id, sort_key_t key)
{
    view_sort_t *sort = sort_get(lib, playlist_id);

    if (!sort)
        return;
    if (sort->key == key)
        sort->desc = !sort->desc;
    else {
        sort->key = key;
        sort->desc = sort_defs[key].desc_first;
    }
    if (key == SORT_CUSTOM)
        sort->desc = 0;
}

/*
** forgets the sort of a playlist, or every sort if playlist_id is -1
*/
void sort_remove(library_t *lib, int playlist_id)
{
    view_sort_t **cur = &lib->sorts;
    view_sort_t *old = NULL;

    while (*cur) {
        if (playlist_id != -1 && (*cur)->playlist_id != playlist_id) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old->custom);
        free(old);
    }
}
