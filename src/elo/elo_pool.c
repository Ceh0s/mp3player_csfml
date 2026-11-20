/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** pool of an arena: the songs or artists that can meet in its battles
** and appear in its rankings
** songs   : the present songs of the ranking
** artists : every artist linked to one of these songs
*/

#include "mp3player.h"

static int pool_has(pool_t const *pool, song_t const *song,
    artist_t const *artist)
{
    for (int i = 0; i < pool->nb; i++)
        if (pool->items[i].song == song && pool->items[i].artist == artist)
            return 1;
    return 0;
}

static void add_songs(library_t *lib, int board, pool_t *pool)
{
    for (int i = 0; i < lib->nb; i++) {
        if (!board_has_song(lib, board, lib->present[i]))
            continue;
        pool->items[pool->nb].song = lib->present[i];
        pool->nb++;
    }
}

/*
** songs holds the songs of the ranking, the artists are added to pool
*/
static void add_artists(library_t *lib, pool_t const *songs, pool_t *pool)
{
    song_t *song = NULL;
    artist_t *artist = NULL;

    for (link_t *link = lib->links; link; link = link->next) {
        song = song_by_id(lib, link->song_id);
        artist = artist_by_id(lib, link->artist_id);
        if (!song || !artist || !pool_has(songs, song, NULL)
            || pool_has(pool, NULL, artist))
            continue;
        pool->items[pool->nb].artist = artist;
        pool->nb++;
    }
}

static int count_artists(library_t *lib)
{
    int nb = 0;

    for (artist_t *artist = lib->artists; artist; artist = artist->next)
        nb++;
    return nb;
}

/*
** fills pool (its items are to free), returns 84 on error
*/
int pool_build(library_t *lib, arena_t const *arena, pool_t *pool)
{
    pool_t songs = {calloc(lib->nb + 1, sizeof(rank_t)), 0};

    pool->items = NULL;
    pool->nb = 0;
    if (!songs.items)
        return 84;
    add_songs(lib, arena->board, &songs);
    if (arena->kind == KIND_SONG) {
        *pool = songs;
        return 0;
    }
    pool->items = calloc(count_artists(lib) + 1, sizeof(rank_t));
    if (pool->items)
        add_artists(lib, &songs, pool);
    free(songs.items);
    return pool->items ? 0 : 84;
}
