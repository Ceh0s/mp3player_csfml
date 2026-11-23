/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** listening to a song outside the player (battle, duplicates, see
** src/audio/preview.c, nothing is counted): "Écouter" / "Pause" button,
** progress bar where a click moves in the song, and the time
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor yellow = {255, 200, 0, 255};

/*
** 1 if the song is the one being previewed
*/
int listen_mine(app_t *app, song_t const *song)
{
    return app->player->preview && app->player->preview_song == song;
}

/*
** returns 1 if clicked (the preview starts, pauses or goes on)
*/
int listen_button(app_t *app, song_t *song, float const *rect)
{
    int playing = listen_mine(app, song)
        && sfMusic_getStatus(app->player->preview) == sfPlaying;

    if (!widget_button(app, rect, playing ? "Pause" : "Écouter"))
        return 0;
    preview_toggle(app, song);
    return 1;
}

/*
** pos = {x, y, size of the characters}
*/
void listen_time(app_t *app, float const *pos)
{
    sfMusic *preview = app->player->preview;
    char line[48];
    char now[16];
    char total[16];

    if (!preview)
        return;
    format_time(now, sizeof(now),
        sfTime_asMilliseconds(sfMusic_getPlayingOffset(preview)));
    format_time(total, sizeof(total),
        sfTime_asMilliseconds(sfMusic_getDuration(preview)));
    snprintf(line, sizeof(line), "%s / %s", now, total);
    widget_text(app, line, pos, &gray);
}

/*
** the bar is thin, the zone of the click is higher; returns 1 if clicked
*/
int listen_bar(app_t *app, float const *bar)
{
    sfMusic *preview = app->player->preview;
    float zone[4] = {bar[0], bar[1] - 12, bar[2], 30};
    float done[4] = {bar[0], bar[1], bar[2], bar[3]};
    sfColor back = {70, 70, 70, 255};
    float ratio = 0;

    if (!preview)
        return 0;
    if (app->wg->mode == WG_CLICK && mouse_in_rect(app, zone)) {
        app->wg->mode = WG_IDLE;
        preview_seek(app, (app->mouse.x - bar[0]) / bar[2]);
        return 1;
    }
    ratio = sfTime_asSeconds(sfMusic_getPlayingOffset(preview))
        / sfTime_asSeconds(sfMusic_getDuration(preview));
    widget_rect(app, bar, &back);
    done[2] *= ratio > 0 ? ratio : 0;
    widget_rect(app, done, &yellow);
    return 0;
}
