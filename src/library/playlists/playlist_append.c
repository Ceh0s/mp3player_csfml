/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** adds a song at the end of playlists without ever removing it (unlike
** playlist_toggle): used by the download for its target playlists
*/

#include "mp3player.h"

/*
** song is a resolved song; returns 1 if it was added
*/
int playlist_append(library_t *lib, playlist_t *playlist, song_t *song)
{
    if (!playlist || !song || playlist_has_song(lib, playlist, song))
        return 0;
    return playlist_toggle(lib, playlist, song) == 0;
}

/*
** ids : the playlists (a removed one is ignored); returns how many
** playlists received the song
*/
int playlists_append(library_t *lib, int const *ids, int nb, song_t *song)
{
    int added = 0;

    for (int i = 0; i < nb; i++)
        added += playlist_append(lib, playlist_by_id(lib, ids[i]), song);
    return added;
}
