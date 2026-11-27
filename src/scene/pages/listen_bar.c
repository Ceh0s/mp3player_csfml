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

    if (!widget_icon_button(app, rect, playing ? ICON_PAUSE : ICON_PLAY,
            playing ? "Pause" : "Écouter"))
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
** position of the mouse on the bar, from 0 to 1
*/
static float mouse_ratio(app_t *app, float const *bar)
{
    float ratio = (app->mouse.x - bar[0]) / bar[2];

    if (ratio < 0)
        return 0;
    return ratio > 1 ? 1 : ratio;
}

/*
** a press on the bar holds its cursor, which follows the mouse; the song
** moves there when it is released (not a click on what is under it:
** app->scroll_held); returns the ratio drawn
*/
static float cursor_drag(app_t *app, float const *zone, float const *bar,
    float ratio)
{
    player_t *player = app->player;
    int down = sfMouse_isButtonPressed(sfMouseLeft);

    if (down && !player->was_down && mouse_in_rect(app, zone))
        player->drag = 1;
    player->was_down = down;
    if (!player->drag)
        return ratio;
    app->scroll_held = 1;
    if (down)
        return mouse_ratio(app, bar);
    player->drag = 0;
    preview_seek(app, mouse_ratio(app, bar));
    return mouse_ratio(app, bar);
}

/*
** the bar is thin, the zone of the click is higher; a click or a drag
** moves in the song; returns 1 if clicked
*/
int listen_bar(app_t *app, float const *bar)
{
    sfMusic *preview = app->player->preview;
    float zone[4] = {bar[0] - 10, bar[1] - 14, bar[2] + 20, bar[3] + 28};
    float done[4] = {bar[0], bar[1], bar[2], bar[3]};
    sfColor back = {70, 70, 70, 255};
    float ratio = 0;

    if (!preview)
        return 0;
    if (app->wg->mode == WG_CLICK && mouse_in_rect(app, zone)) {
        app->wg->mode = WG_IDLE;
        preview_seek(app, mouse_ratio(app, bar));
        return 1;
    }
    if (app->wg->mode != WG_DRAW)
        return 0;
    ratio = sfTime_asSeconds(sfMusic_getPlayingOffset(preview))
        / sfTime_asSeconds(sfMusic_getDuration(preview));
    ratio = cursor_drag(app, zone, bar, ratio > 0 ? ratio : 0);
    widget_rect(app, bar, &back);
    done[2] *= ratio;
    widget_rect(app, done, &yellow);
    widget_dot(app, bar[0] + done[2], bar[1] + bar[3] / 2,
        app->player->drag ? 11 : 8, &yellow);
    return 0;
}
