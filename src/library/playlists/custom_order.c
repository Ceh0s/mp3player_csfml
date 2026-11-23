/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** custom order of a playlist: the songs moved by hand in the library
** (saved apart in custom_orders.tsv, as raw song ids)
*/

#include "mp3player.h"

/*
** position in the custom order (custom holds its resolved songs); a song
** not placed yet comes after, in the order of the playlist
*/
long custom_position(view_sort_t const *sort, song_t *const *custom,
    song_t const *song, int native)
{
    for (int i = 0; i < sort->nb_custom; i++)
        if (custom[i] == song)
            return i;
    return sort->nb_custom + native;
}

/*
** the custom order becomes the order of view; the songs that are not in
** the music folder now keep their place at the end, nothing is lost
*/
int sort_set_custom(library_t *lib, view_sort_t *sort, song_t *const *view,
    int nb)
{
    int *ids = malloc(sizeof(int) * (nb + sort->nb_custom + 1));
    int count = 0;
    song_t *song = NULL;

    if (!ids)
        return 84;
    for (; count < nb; count++)
        ids[count] = view[count]->id;
    for (int i = 0; i < sort->nb_custom; i++) {
        song = song_resolve(lib, song_by_id(lib, sort->custom[i]));
        if (song && song->present)
            continue;
        ids[count] = sort->custom[i];
        count++;
    }
    free(sort->custom);
    sort->custom = ids;
    sort->nb_custom = count;
    return 0;
}
