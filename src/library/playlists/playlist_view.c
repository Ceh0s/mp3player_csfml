/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** the songs to show and play for a playlist: the present ones, in the
** order of the playlist, each one once (two merged ids give one song)
*/

#include "mp3player.h"

static int already_in(song_t *const *view, int nb, song_t const *song)
{
    for (int i = 0; i < nb; i++)
        if (view[i] == song)
            return 1;
    return 0;
}

/*
** view must hold lib->nb + 1 songs; playlist = NULL gives every present
** song ("Tous les titres"); returns the number of songs
*/
int playlist_view(library_t *lib, playlist_t const *playlist,
    song_t **view)
{
    song_t *song = NULL;
    int nb = 0;

    if (!playlist) {
        memcpy(view, lib->present, sizeof(song_t *) * (lib->nb + 1));
        return lib->nb;
    }
    for (int i = 0; i < playlist->nb && nb < lib->nb; i++) {
        song = song_resolve(lib, song_by_id(lib, playlist->songs[i]));
        if (!song || !song->present || already_in(view, nb, song))
            continue;
        view[nb] = song;
        nb++;
    }
    view[nb] = NULL;
    return nb;
}
