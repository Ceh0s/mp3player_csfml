/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** rankings screen: tier list and leaderboard of a category
** (drawn with the widgets, like the edit screen)
*/

#include "mp3player.h"

int ranking_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};

    app->ranking = calloc(1, sizeof(ranking_t));
    if (!app->ranking)
        return 84;
    app->ranking->bg = background_create(BG_BLUR_PATH,
        &app->ranking->bg_texture);
    app->ranking->panel = rect_create(panel, &dark);
    if (!app->ranking->bg || !app->ranking->panel)
        return 84;
    ranking_build(app);
    return 0;
}

void ranking_event(app_t *app, sfEvent *event)
{
    if (event->type == sfEvtMouseWheelScrolled
        && app->ranking->tab == TAB_BOARD)
        ranking_scroll(app, event->mouseWheelScroll.delta);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    ranking_view(app);
    app->wg->mode = WG_DRAW;
}

void ranking_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->ranking->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->ranking->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    ranking_view(app);
}

void ranking_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->ranking->bg);
    sfRectangleShape_setSize(app->ranking->panel, size);
    ranking_scroll(app, 0);
}

void ranking_destroy(app_t *app)
{
    if (!app->ranking)
        return;
    if (app->ranking->bg)
        sfSprite_destroy(app->ranking->bg);
    if (app->ranking->bg_texture)
        sfTexture_destroy(app->ranking->bg_texture);
    if (app->ranking->panel)
        sfRectangleShape_destroy(app->ranking->panel);
    free(app->ranking->ranks);
    free(app->ranking);
    app->ranking = NULL;
}
