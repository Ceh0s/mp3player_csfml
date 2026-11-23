/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** listening to a song during a battle: a second sfMusic, apart from the
** player, that counts nothing (no listen logged, no play count, no
** listening time); the music of the player is paused meanwhile and
** goes on when the preview stops
*/

#include "mp3player.h"

static void preview_close(player_t *player)
{
    if (player->preview) {
        sfMusic_stop(player->preview);
        sfMusic_destroy(player->preview);
    }
    player->preview = NULL;
    player->preview_song = NULL;
}

/*
** the player goes on if it was playing before the preview
*/
void preview_stop(app_t *app)
{
    player_t *player = app->player;

    preview_close(player);
    if (player->resume && player->music)
        sfMusic_play(player->music);
    player->resume = 0;
}

/*
** another preview may be playing: the player stays paused
*/
static void preview_start(app_t *app, song_t *song)
{
    player_t *player = app->player;

    preview_close(player);
    if (song->path)
        player->preview = sfMusic_createFromFile(song->path);
    if (!player->preview) {
        preview_stop(app);
        return;
    }
    player->preview_song = song;
    if (player->music && sfMusic_getStatus(player->music) == sfPlaying) {
        sfMusic_pause(player->music);
        player->resume = 1;
    }
    sfMusic_play(player->preview);
}

/*
** plays the song from its start, or pauses / goes on if it is the one
** being previewed
*/
void preview_toggle(app_t *app, song_t *song)
{
    sfMusic *preview = app->player->preview;

    if (!preview || app->player->preview_song != song) {
        preview_start(app, song);
        return;
    }
    if (sfMusic_getStatus(preview) == sfPlaying)
        sfMusic_pause(preview);
    else
        sfMusic_play(preview);
}

/*
** ratio : where to go, from 0 (start) to 1 (end)
*/
void preview_seek(app_t *app, float ratio)
{
    sfMusic *preview = app->player->preview;
    sfTime length = {0};

    if (!preview)
        return;
    length = sfMusic_getDuration(preview);
    sfMusic_setPlayingOffset(preview,
        sfMicroseconds(sfTime_asMicroseconds(length) * ratio));
}

/*
** a preview that reached its end stops (the player goes on)
*/
void preview_update(app_t *app)
{
    if (app->player->preview
        && sfMusic_getStatus(app->player->preview) == sfStopped)
        preview_stop(app);
}
