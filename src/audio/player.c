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

/*
** the song at index of the queue becomes the current one
*/
static void set_current(player_t *player, song_t *song, int index)
{
    player->index = index;
    player->song = song;
    player->session_ms = 0;
    player->carry_us = 0;
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
    set_current(player, song, index);
    song->play_count++;
    song->duration_ms = sfTime_asMilliseconds(
        sfMusic_getDuration(player->music));
    player_volume_apply(app);
    sfMusic_play(player->music);
}

/*
** the song is looked for in the queue (in a random order, its place is
** not the one of the list shown); nothing if it is not in it
*/
void player_play_song(app_t *app, song_t const *song)
{
    for (int i = 0; i < app->player->nb; i++)
        if (app->player->queue[i] == song) {
            player_play(app, i);
            return;
        }
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

/*
** with the queue repeated, the last song is followed by the first one
** and the first one preceded by the last one
*/
void player_skip(app_t *app, int direction)
{
    player_t *player = app->player;
    int target = player->index + direction;

    if (app->lib->settings.repeat == REPEAT_QUEUE && player->nb > 0)
        target = (target + player->nb) % player->nb;
    if (!player->music || target < 0 || target >= player->nb)
        return;
    player_play(app, target);
}
