/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** finds songs by id and follows merges
*/

#include "mp3player.h"

song_t *song_by_id(library_t *lib, int id)
{
    for (song_t *song = lib->songs; song; song = song->next)
        if (song->id == id)
            return song;
    return NULL;
}

/*
** a merged song is only an alias: returns the song it was merged into
** (the count stops an infinite loop if the save files were corrupted)
*/
song_t *song_resolve(library_t *lib, song_t *song)
{
    song_t *target = NULL;

    for (int count = 0; song && song->merged_into && count < 64; count++) {
        target = song_by_id(lib, song->merged_into);
        if (!target)
            return song;
        song = target;
    }
    return song;
}

/*
** own listening time + the one of every song merged into it
*/
long song_total_listen(library_t *lib, song_t const *song)
{
    long total = song->listen_ms;

    for (song_t *other = lib->songs; other; other = other->next)
        if (other->merged_into && other != song
            && song_resolve(lib, other) == song)
            total += other->listen_ms;
    return total;
}
