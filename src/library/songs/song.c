/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** creation and destruction of a song
*/

#include "mp3player.h"

song_t *song_create(void)
{
    song_t *song = calloc(1, sizeof(song_t));

    return song;
}

void song_destroy(song_t *song)
{
    if (!song)
        return;
    free(song->key);
    free(song->hash);
    free(song->file);
    free(song->path);
    free(song->title);
    free(song->artists);
    free(song->album);
    free(song->genre);
    free(song->year);
    free(song->src_title);
    free(song->src_artists);
    free(song);
}

void song_list_destroy(song_t *song)
{
    song_t *next = NULL;

    while (song) {
        next = song->next;
        song_destroy(song);
        song = next;
    }
}

char const *song_title(song_t const *song)
{
    if (song->title)
        return song->title;
    if (song->src_title)
        return song->src_title;
    if (song->file)
        return song->file;
    return "?";
}
