/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** playlists: creation, search, removal, songs added or removed;
** they keep the raw song ids, resolved when used (a merge then puts the
** merged song in the playlist, a split restores it)
*/

#include "mp3player.h"

/*
** appended at the end of the list; returns NULL if the id is used
*/
playlist_t *playlist_add(library_t *lib, int id, char const *name)
{
    playlist_t **last = &lib->playlists;
    playlist_t *playlist = NULL;

    if (id <= 0 || playlist_by_id(lib, id))
        return NULL;
    playlist = calloc(1, sizeof(playlist_t));
    if (!playlist)
        return NULL;
    playlist->id = id;
    playlist->name = strdup(name);
    if (!playlist->name) {
        free(playlist);
        return NULL;
    }
    while (*last)
        last = &(*last)->next;
    *last = playlist;
    if (id >= lib->next_playlist)
        lib->next_playlist = id + 1;
    return playlist;
}

playlist_t *playlist_by_id(library_t *lib, int id)
{
    for (playlist_t *pl = lib->playlists; pl; pl = pl->next)
        if (pl->id == id)
            return pl;
    return NULL;
}

/*
** playlist = NULL frees all of them
*/
void playlist_remove(library_t *lib, playlist_t *playlist)
{
    playlist_t **cur = &lib->playlists;
    playlist_t *old = NULL;

    while (*cur) {
        if (playlist && *cur != playlist) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old->songs);
        free(old->name);
        free(old);
    }
}

/*
** song is a resolved song
*/
int playlist_has_song(library_t *lib, playlist_t const *playlist,
    song_t const *song)
{
    for (int i = 0; i < playlist->nb; i++)
        if (song_resolve(lib, song_by_id(lib, playlist->songs[i])) == song)
            return 1;
    return 0;
}

/*
** adds the song at the end, or removes it (with every id that resolves
** to it) if it is already in the playlist
*/
int playlist_toggle(library_t *lib, playlist_t *playlist, song_t *song)
{
    int kept = 0;
    int *bigger = NULL;

    if (!playlist_has_song(lib, playlist, song)) {
        bigger = realloc(playlist->songs, sizeof(int) * (playlist->nb + 1));
        if (!bigger)
            return 84;
        playlist->songs = bigger;
        playlist->songs[playlist->nb] = song->id;
        playlist->nb++;
        return 0;
    }
    for (int i = 0; i < playlist->nb; i++) {
        if (song_resolve(lib, song_by_id(lib, playlist->songs[i])) == song)
            continue;
        playlist->songs[kept] = playlist->songs[i];
        kept++;
    }
    playlist->nb = kept;
    return 0;
}
