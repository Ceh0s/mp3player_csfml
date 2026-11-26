/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** result of a battle and choice of the next pair (or the same one
** again after the edit of one of its songs)
*/

#include "mp3player.h"

/*
** the preview of the pair stops, the arrows choose again; a new
** category is drawn if "Catégorie aléatoire" is on
*/
void battle_next(app_t *app)
{
    battle_t *battle = app->battle;

    preview_stop(app);
    battle->focus = -1;
    if (app->lib->settings.battle_random)
        app->arena.category = category_random(app->lib, app->arena.category);
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

static rank_t *pool_find(pool_t *pool, int id)
{
    for (int i = 0; i < pool->nb; i++)
        if (pool->items[i].song && pool->items[i].song->id == id)
            return &pool->items[i];
    return NULL;
}

/*
** back from the edit of a song of the pair (right click): the same pair,
** unless one of them left the pool (removed, excluded or merged)
*/
void battle_restore(app_t *app)
{
    battle_t *battle = app->battle;
    rank_t *left = pool_find(&battle->pool, app->battle_keep[0]);
    rank_t *right = pool_find(&battle->pool, app->battle_keep[1]);

    app->battle_keep[0] = 0;
    app->battle_keep[1] = 0;
    if (!left || !right)
        return;
    battle->pair[0] = *left;
    battle->pair[1] = *right;
    battle->has_pair = 1;
}
