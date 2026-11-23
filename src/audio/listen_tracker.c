/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** counts the real time spent listening (pauses excluded)
** and logs every listening session in data/listens.log
*/

#include "mp3player.h"

void listen_log(song_t const *song, long ms, end_reason_t reason)
{
    char const *names[3] = {"complete", "skip", "quit"};
    char line[128];

    snprintf(line, sizeof(line), "%d\t%ld\t%s", song->id, ms,
        names[reason]);
    log_line(LISTENS_LOG, LISTENS_COLUMNS, line);
}

static void count_time(app_t *app, song_t *song)
{
    player_t *player = app->player;
    long ms = 0;

    player->carry_us += app->delta_us;
    ms = player->carry_us / 1000;
    player->carry_us %= 1000;
    player->session_ms += ms;
    song->listen_ms += ms;
}

/*
** the song after index once one ended: the same one (REPEAT_ONE), the
** first one after the last (REPEAT_QUEUE, drawn again if random), or
** nb (none)
*/
static int next_song(app_t *app, int index)
{
    player_t *player = app->player;
    settings_t const *settings = &app->lib->settings;

    if (settings->repeat == REPEAT_ONE)
        return index;
    if (index + 1 < player->nb || settings->repeat != REPEAT_QUEUE)
        return index + 1;
    if (settings->shuffle)
        queue_shuffle(player->queue, player->nb, NULL);
    return 0;
}

/*
** A music that stops on its own reached its end: the next one starts.
*/
void player_update(app_t *app)
{
    player_t *player = app->player;
    sfSoundStatus status = sfStopped;
    int index = player->index;

    preview_update(app);
    if (!player->music)
        return;
    status = sfMusic_getStatus(player->music);
    if (status == sfPlaying)
        count_time(app, player->song);
    if (status != sfStopped)
        return;
    player_stop(app, END_COMPLETE);
    index = next_song(app, index);
    if (index < player->nb)
        player_play(app, index);
}
