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

    battle->has_pair = elo_pick_pair(app->lib, &app->arena, &battle->pool,
        battle->pair);
}

/*
** the ranking or the kind changed: new pool, and a pair from it
*/
void battle_reload(app_t *app)
{
    battle_t *battle = app->battle;

    free(battle->pool.items);
    if (!board_by_id(app->lib, app->arena.board))
        app->arena.board = BOARD_GENERAL;
    pool_build(app->lib, &app->arena, &battle->pool);
    memset(battle->pair, 0, sizeof(battle->pair));
    battle_next(app);
}

/*
** side : 0 for the one on the left, 1 for the one on the right
*/
void battle_vote(app_t *app, int side)
{
    battle_t *battle = app->battle;

    if (!battle->has_pair)
        return;
    battle_record(app->lib, &app->arena, &battle->pair[side],
        &battle->pair[1 - side]);
    battle_next(app);
}
