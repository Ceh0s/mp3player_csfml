/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** buttons of a battle card: likes (information only, no effect on the
** Elo), listening under the card, out of the zone of the vote
** (src/scene/pages/listen_bar.c), and the frame of the card chosen with
** the arrows (confirmed with Enter)
*/

#include "mp3player.h"

static const sfColor yellow = {255, 200, 0, 255};

/*
** likes on the card; returns 1 if a click was used here
*/
int battle_likes(app_t *app, rank_t const *rank, float x)
{
    return like_buttons(app, rank->song ? KIND_SONG : KIND_ARTIST,
        rank_id(rank), (float [3]){x + 30, BATTLE_CARD_Y + 20, 1});
}

/*
** listening to a song, under its card: a click or a drag there is never
** a vote; returns 1 if a click was used here
*/
int battle_listen(app_t *app, rank_t const *rank, float x)
{
    float y = BATTLE_LISTEN_Y;

    if (!rank->song)
        return 0;
    if (listen_button(app, rank->song, (float [4]){x, y, 170, 44}))
        return 1;
    if (!listen_mine(app, rank->song))
        return 0;
    if (listen_bar(app, (float [4]){x + 196, y + 19, 430, 6}))
        return 1;
    listen_time(app, (float [3]){x + 648, y + 8, 22});
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
