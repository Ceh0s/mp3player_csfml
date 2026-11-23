/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** the two automatic playlists "Titres likés" and "Titres dislikés":
** real playlists (auto_like = 1 / -1) so that they work everywhere
** (tabs, player, rankings), made again from the likes after each change
** of them; they cannot be edited by hand nor removed
*/

#include "mp3player.h"

playlist_t *playlist_auto(library_t *lib, int value)
{
    for (playlist_t *pl = lib->playlists; pl; pl = pl->next)
        if (pl->auto_like == value)
            return pl;
    return NULL;
}

/*
** created if missing (first launch, or file edited by hand)
*/
static playlist_t *auto_get(library_t *lib, int value)
{
    playlist_t *pl = playlist_auto(lib, value);

    if (pl)
        return pl;
    pl = playlist_add(lib, lib->next_playlist,
        value > 0 ? "Titres likés" : "Titres dislikés");
    if (pl) {
        pl->auto_like = value;
        boards_sync_playlists(lib);
    }
    return pl;
}

/*
** the songs liked (or disliked), in the order they were
*/
static void fill(library_t *lib, playlist_t *pl, int value)
{
    int nb = 0;

    for (like_t *like = lib->likes; like; like = like->next)
        nb += like->kind == KIND_SONG && like->value == value;
    free(pl->songs);
    pl->songs = malloc(sizeof(int) * (nb + 1));
    pl->nb = 0;
    for (like_t *like = lib->likes; pl->songs && like; like = like->next) {
        if (like->kind != KIND_SONG || like->value != value)
            continue;
        pl->songs[pl->nb] = like->id;
        pl->nb++;
    }
}

void likes_sync(library_t *lib)
{
    playlist_t *liked = auto_get(lib, 1);
    playlist_t *disliked = auto_get(lib, -1);

    if (liked)
        fill(lib, liked, 1);
    if (disliked)
        fill(lib, disliked, -1);
}
