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
    return 0;
}

/*
** left and right arrows vote for the song on that side
*/
void battle_event(app_t *app, sfEvent *event)
{
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyLeft)
        battle_vote(app, 0);
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyRight)
        battle_vote(app, 1);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    battle_view(app);
    app->wg->mode = WG_DRAW;
}

void battle_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->battle->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->battle->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    battle_view(app);
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
