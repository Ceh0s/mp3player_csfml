/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** plays one song at a time: only the current sfMusic is opened
*/

#include "mp3player.h"

void player_stop(app_t *app, end_reason_t reason)
{
    player_t *player = app->player;
    song_t *song = NULL;

    if (!player->music)
        return;
    song = player->song;
    if (reason == END_SKIP)
        song->skip_count++;
    listen_log(song, player->session_ms, reason);
    sfMusic_stop(player->music);
    sfMusic_destroy(player->music);
    player->music = NULL;
    player->song = NULL;
    player->index = -1;
    if (reason != END_QUIT)
        db_save_all(app->lib);
}

void player_play(app_t *app, int index)
{
    player_t *player = app->player;
    song_t *song = NULL;

    if (index < 0 || index >= player->nb)
        return;
    song = player->queue[index];
    player_stop(app, END_SKIP);
    player->music = sfMusic_createFromFile(song->path);
    if (!player->music) {
        fprintf(stderr, "Cannot play %s\n", song->path);
        return;
    }
    player->index = index;
    player->song = song;
    player->session_ms = 0;
    player->carry_us = 0;
    song->play_count++;
    song->duration_ms = sfTime_asMilliseconds(
        sfMusic_getDuration(player->music));
    sfMusic_play(player->music);
}

/*
** during a battle preview, Space pauses the preview instead
*/
void player_toggle(app_t *app)
{
    sfMusic *music = app->player->music;

    if (app->player->preview) {
        preview_toggle(app, app->player->preview_song);
        return;
    }
    if (!music)
        return;
    if (sfMusic_getStatus(music) == sfPlaying)
        sfMusic_pause(music);
    else
        sfMusic_play(music);
}

void player_skip(app_t *app, int direction)
{
    int target = app->player->index + direction;

    if (!app->player->music || target < 0 || target >= app->player->nb)
        return;
    player_play(app, target);
}
