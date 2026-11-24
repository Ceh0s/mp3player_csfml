/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** opens the page of a song (edit screen) or of an artist (the edit
** screen on its artist page), from a right click on a list (rankings,
** statistics); scene_back comes back to the list
*/

#include "mp3player.h"

/*
** returns 0 if rank holds neither a song nor an artist
*/
int page_open(app_t *app, rank_t const *rank)
{
    if (!rank->song && !rank->artist)
        return 0;
    app->edit_song = rank->song;
    app->edit_artist = rank->song ? NULL : rank->artist;
    scene_open(app, EDIT);
    return 1;
}
