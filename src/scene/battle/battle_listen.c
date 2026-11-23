/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** buttons of a battle card: likes (information only, no effect on the
** Elo) and listening ("Écouter", progress bar where a click moves in
** the song, time), and the frame of the card chosen with the arrows
** (confirmed with Enter)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor yellow = {255, 200, 0, 255};

static void listen_time(app_t *app, sfMusic *preview, float x)
{
    char line[48];
    char now[16];
    char total[16];

    format_time(now, sizeof(now),
        sfTime_asMilliseconds(sfMusic_getPlayingOffset(preview)));
    format_time(total, sizeof(total),
        sfTime_asMilliseconds(sfMusic_getDuration(preview)));
    snprintf(line, sizeof(line), "%s / %s", now, total);
    widget_text(app, line, (float [3]){x + 30, BATTLE_CARD_Y + 80, 22},
        &gray);
}

/*
** the bar is thin, the zone of the click is higher; returns 1 if clicked
*/
static int seek_bar(app_t *app, sfMusic *preview, float x)
{
    float bar[4] = {x + 30, BATTLE_CARD_Y + BATTLE_CARD_H - 14, 700, 6};
    float zone[4] = {bar[0], bar[1] - 12, bar[2], 30};
    sfColor back = {70, 70, 70, 255};
    float ratio = 0;

    if (app->wg->mode == WG_CLICK && mouse_in_rect(app, zone)) {
        app->wg->mode = WG_IDLE;
        preview_seek(app, (app->mouse.x - bar[0]) / bar[2]);
        return 1;
    }
    ratio = sfTime_asSeconds(sfMusic_getPlayingOffset(preview))
        / sfTime_asSeconds(sfMusic_getDuration(preview));
    widget_rect(app, bar, &back);
    bar[2] *= ratio > 0 ? ratio : 0;
    widget_rect(app, bar, &yellow);
    return 0;
}

/*
** only for a song; returns 1 if a click was used here
*/
int battle_listen(app_t *app, rank_t const *rank, float x)
{
    sfMusic *preview = app->player->preview;
    int mine = preview && app->player->preview_song == rank->song;
    int playing = mine && sfMusic_getStatus(preview) == sfPlaying;

    if (like_buttons(app, rank->song ? KIND_SONG : KIND_ARTIST,
            rank_id(rank), (float [3]){x + 30, BATTLE_CARD_Y + 120, 1}))
        return 1;
    if (!rank->song)
        return 0;
    if (widget_button(app, (float [4]){x + 30, BATTLE_CARD_Y + 20, 170, 50},
            playing ? "Pause" : "Écouter")) {
        preview_toggle(app, rank->song);
        return 1;
    }
    if (!mine)
        return 0;
    if (seek_bar(app, preview, x))
        return 1;
    listen_time(app, preview, x);
    return 0;
}

/*
** frame around the card at x
*/
void battle_frame(app_t *app, float x)
{
    float y = BATTLE_CARD_Y;
    float h = BATTLE_CARD_H;

    widget_rect(app, (float [4]){x - 4, y - 4, 768, 4}, &yellow);
    widget_rect(app, (float [4]){x - 4, y + h, 768, 4}, &yellow);
    widget_rect(app, (float [4]){x - 4, y, 4, h}, &yellow);
    widget_rect(app, (float [4]){x + 760, y, 4, h}, &yellow);
}
