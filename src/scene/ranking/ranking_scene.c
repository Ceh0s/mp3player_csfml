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
    app->ranking->drag_row = -1;
    app->ranking->tab = TAB_BOARD;
    app->ranking->open_tier = -1;
    ranking_build(app);
    ranking_restore(app);
    return 0;
}

/*
** Escape closes the reset window first; it receives the typed text
*/
static void ranking_key(app_t *app, sfEvent *event)
{
    ranking_t *ranking = app->ranking;

    if (event->type == sfEvtTextEntered && ranking->reset)
        text_input_n(app, ranking->confirm, sizeof(ranking->confirm),
            event->text.unicode);
    if (event->type != sfEvtKeyPressed || event->key.code != sfKeyEscape)
        return;
    if (!ranking->reset && ranking->open_tier >= 0
        && ranking->tab == TAB_TIERS) {
        ranking->open_tier = -1;
        ranking->scroll = 0;
        return;
    }
    if (!ranking->reset) {
        scene_back(app);
        return;
    }
    ranking->reset = 0;
    ranking->confirm[0] = '\0';
}

void ranking_event(app_t *app, sfEvent *event)
{
    ranking_key(app, event);
    if (event->type == sfEvtMouseWheelScrolled
        && app->ranking->tab != TAB_TIERS)
        ranking_scroll(app, event->mouseWheelScroll.delta);
    if (event->type == sfEvtMouseWheelScrolled
        && app->ranking->tab == TAB_TIERS && app->ranking->open_tier >= 0)
        app->ranking->scroll -= (int)event->mouseWheelScroll.delta;
    if (event->type == sfEvtMouseButtonPressed && !app->ranking->reset
        && event->mouseButton.button == sfMouseRight) {
        ranking_open_hover(app);
        return;
    }
    if (event->type == sfEvtMouseButtonPressed && !app->ranking->reset
        && app->ranking->tab == TAB_TIES && app->search.text[0] == '\0')
        ranking_drag_press(app);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    if (app->ranking->tab == TAB_TIES && ranking_drag_release(app))
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
    app->ranking->hover.song = NULL;
    app->ranking->hover.artist = NULL;
    ranking_view(app);
    widget_tip_draw(app);
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
    free(app->ranking->order);
    free(app->ranking->tiers);
    history_free(&app->ranking->history);
    free(app->ranking);
    app->ranking = NULL;
}
