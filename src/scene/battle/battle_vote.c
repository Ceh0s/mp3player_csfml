/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** result of a battle and choice of the next pair
*/

#include "mp3player.h"

void battle_next(app_t *app)
{
    battle_t *battle = app->battle;

    battle->has_pair = elo_pick_pair(app->lib, battle->category,
        battle->pair);
}

/*
** side : 0 for the song on the left, 1 for the one on the right
*/
void battle_vote(app_t *app, int side)
{
    battle_t *battle = app->battle;

    if (!battle->has_pair)
        return;
    battle_record(app->lib, battle->pair[side], battle->pair[1 - side],
        battle->category);
    battle_next(app);
}
