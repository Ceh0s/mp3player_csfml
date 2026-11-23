/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** duration of the songs: few files have it in their tags, so it is
** measured once by CSFML for each present song without one, then saved
** in songs.tsv (-1 if the file cannot be opened: not tried again)
*/

#include "mp3player.h"

void library_fill_durations(library_t *lib)
{
    song_t *song = NULL;
    sfMusic *music = NULL;

    for (int i = 0; i < lib->nb; i++) {
        song = lib->present[i];
        if (song->duration_ms != 0 || !song->path)
            continue;
        music = sfMusic_createFromFile(song->path);
        song->duration_ms = -1;
        if (!music)
            continue;
        song->duration_ms = sfTime_asMilliseconds(sfMusic_getDuration(music));
        sfMusic_destroy(music);
    }
}
