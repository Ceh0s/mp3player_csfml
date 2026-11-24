/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** buttons of the player bar besides play / pause / skip: go back or
** forward by SEEK_MS, stop (back to the start, paused), and the volume
** of the player and of the battle previews (kept in settings.tsv)
*/

#include "mp3player.h"

/*
** delta : milliseconds to move (negative to go back), kept in the song
*/
void player_seek(app_t *app, long delta)
{
    sfMusic *music = app->player->music;
    long now = 0;
    long total = 0;

    if (!music)
        return;
    now = sfTime_asMilliseconds(sfMusic_getPlayingOffset(music)) + delta;
    total = sfTime_asMilliseconds(sfMusic_getDuration(music));
    if (now < 0)
        now = 0;
    if (now > total - 500)
        now = total - 500;
    sfMusic_setPlayingOffset(music, sfMilliseconds(now));
}

/*
** the song stays the current one (its listening goes on counting when
** it plays again)
*/
void player_halt(app_t *app)
{
    sfMusic *music = app->player->music;

    if (!music)
        return;
    sfMusic_pause(music);
    sfMusic_setPlayingOffset(music, sfMilliseconds(0));
}

/*
** the volume of the settings is given to the musics that are open
*/
void player_volume_apply(app_t *app)
{
    settings_t const *set = &app->lib->settings;
    float volume = set->mute ? 0 : set->volume;

    if (app->player->music)
        sfMusic_setVolume(app->player->music, volume);
    if (app->player->preview)
        sfMusic_setVolume(app->player->preview, volume);
}

/*
** step : VOLUME_STEP or -VOLUME_STEP; a change of volume ends the mute
*/
void player_volume_step(app_t *app, int step)
{
    settings_t *set = &app->lib->settings;

    set->volume += step;
    if (set->volume > 100)
        set->volume = 100;
    if (set->volume < 0)
        set->volume = 0;
    set->mute = 0;
    player_volume_apply(app);
    db_save_settings(app->lib);
}

void player_mute_toggle(app_t *app)
{
    app->lib->settings.mute = !app->lib->settings.mute;
    player_volume_apply(app);
    db_save_settings(app->lib);
}
