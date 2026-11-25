/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** likes of the song being played, in the bottom bar of the library; the
** list follows when an automatic playlist ("Titres likés") is shown
*/

#include "mp3player.h"

void library_likes(app_t *app)
{
    song_t *song = app->player->song;
    playlist_t *shown = playlist_by_id(app->lib, app->libscene->playlist_id);

    if (!song)
        return;
    if (like_buttons(app, KIND_SONG, song->id,
            (float [3]){1420, 992 + app->extra_h, 0})
        && shown && shown->auto_like != 0)
        list_refresh(app);
}
