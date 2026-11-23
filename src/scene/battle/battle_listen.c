/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** buttons of a battle card: likes (information only, no effect on the
** Elo) and listening (src/scene/pages/listen_bar.c), and the frame of
** the card chosen with the arrows (confirmed with Enter)
*/

#include "mp3player.h"

static const sfColor yellow = {255, 200, 0, 255};

/*
** only for a song; returns 1 if a click was used here
*/
int battle_listen(app_t *app, rank_t const *rank, float x)
{
    float bar[4] = {x + 30, BATTLE_CARD_Y + BATTLE_CARD_H - 14, 700, 6};

    if (like_buttons(app, rank->song ? KIND_SONG : KIND_ARTIST,
            rank_id(rank), (float [3]){x + 30, BATTLE_CARD_Y + 120, 1}))
        return 1;
    if (!rank->song)
        return 0;
    if (listen_button(app, rank->song,
            (float [4]){x + 30, BATTLE_CARD_Y + 20, 170, 50}))
        return 1;
    if (!listen_mine(app, rank->song))
        return 0;
    if (listen_bar(app, bar))
        return 1;
    listen_time(app, (float [3]){x + 30, BATTLE_CARD_Y + 80, 22});
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
