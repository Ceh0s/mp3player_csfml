/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** "J'aime" / "Je n'aime pas" of a song or an artist, shared by the
** library bar, the edit screen, the artist page and the battle cards;
** a second click on the chosen one removes it
*/

#include "mp3player.h"

/*
** pos = {x, y, vertical (1: one under the other)}; returns 1 if the
** like changed (the automatic playlists are then made again)
*/
int like_buttons(app_t *app, elo_kind_t kind, int id, float const *pos)
{
    int value = like_get(app->lib, kind, id);
    float like[4] = {pos[0], pos[1], 170, 44};
    float dislike[4] = {pos[0] + 180, pos[1], 200, 44};
    int chosen = 0;

    if (pos[2] != 0) {
        dislike[0] = pos[0];
        dislike[1] = pos[1] + 52;
    }
    if (widget_button(app, like, value > 0 ? "> J'aime <" : "J'aime"))
        chosen = 1;
    else if (widget_button(app, dislike,
            value < 0 ? "> Je n'aime pas <" : "Je n'aime pas"))
        chosen = -1;
    if (chosen == 0)
        return 0;
    like_set(app->lib, kind, id, chosen);
    player_resync(app);
    return 1;
}
