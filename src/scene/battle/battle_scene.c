/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** battle screen: two songs face to face, the user picks the best one
** in the chosen category (drawn with the widgets, like the edit screen)
*/

#include "mp3player.h"

int battle_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};

    app->battle = calloc(1, sizeof(battle_t));
    if (!app->battle)
        return 84;
    app->battle->bg = background_create(BG_BLUR_PATH,
        &app->battle->bg_texture);
    app->battle->panel = rect_create(panel, &dark);
    if (!app->battle->bg || !app->battle->panel)
        return 84;
    if (app->arena.category == CAT_TOTAL)
        app->arena.category = CAT_ALL;
    battle_reload(app);
    battle_restore(app);
    return 0;
}

/*
** left and right arrows choose a side (framed), Enter votes for it
*/
static void battle_key(app_t *app, sfKeyCode code)
{
    battle_t *battle = app->battle;

    if (code == sfKeyLeft)
        battle->focus = 0;
    if (code == sfKeyRight)
        battle->focus = 1;
    if (code == sfKeyEnter && battle->focus >= 0)
        battle_vote(app, battle->focus);
}

/*
** a right click on the card of a song opens its edit screen (to remove
** it, for example); the battle is built again when coming back, with
** the same pair if both songs are still in it
*/
/*
** the rankings screen on its "Historique" tab; the same pair is shown
** again when coming back (battle_restore)
*/
void battle_open_history(app_t *app)
{
    battle_t *battle = app->battle;
    ranking_keep_t keep = {1, TAB_HISTORY, 0, -1};

    preview_stop(app);
    if (battle->has_pair && battle->pair[0].song && battle->pair[1].song) {
        app->battle_keep[0] = battle->pair[0].song->id;
        app->battle_keep[1] = battle->pair[1].song->id;
    }
    app->ranking_keep = keep;
    scene_open(app, RANKING);
}

static void right_click(app_t *app)
{
    battle_t *battle = app->battle;
    float card[4] = {130, BATTLE_CARD_Y, 760, BATTLE_CARD_H};

    for (int i = 0; battle->has_pair && i < 2; i++) {
        card[0] = i == 0 ? 130 : 1030;
        if (!battle->pair[i].song || !mouse_in_rect(app, card))
            continue;
        preview_stop(app);
        app->battle_keep[0] = battle->pair[0].song->id;
        app->battle_keep[1] = battle->pair[1].song->id;
        app->edit_song = battle->pair[i].song;
        scene_open(app, EDIT);
    }
}

void battle_event(app_t *app, sfEvent *event)
{
    if (catlist_event(app, event))
        return;
    if (event->type == sfEvtKeyPressed)
        battle_key(app, event->key.code);
    if (event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseRight)
        right_click(app);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    battle_view(app);
    app->wg->mode = WG_DRAW;
}

void battle_draw(app_t *app)
{
    background_draw(app, app->battle->bg);
    sfRenderWindow_drawRectangleShape(app->window, app->battle->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    battle_view(app);
    widget_tip_draw(app);
}

void battle_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->battle->bg);
    sfRectangleShape_setSize(app->battle->panel, size);
}

void battle_destroy(app_t *app)
{
    if (!app->battle)
        return;
    preview_stop(app);
    if (app->battle->bg)
        sfSprite_destroy(app->battle->bg);
    if (app->battle->bg_texture)
        sfTexture_destroy(app->battle->bg_texture);
    if (app->battle->panel)
        sfRectangleShape_destroy(app->battle->panel);
    free(app->battle->pool.items);
    free(app->battle);
    app->battle = NULL;
}
