/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the rankings screen: category, tabs, buttons
** (called to draw it and to handle a click, see widgets.c)
*/

#include "mp3player.h"

/*
** tier list, leaderboard, order between ex aequo or history; images
** or names in the tier list
*/
static void tab_button(app_t *app, float const *rect, ranking_tab_t tab,
    char const *label)
{
    char text[64];

    snprintf(text, sizeof(text), app->ranking->tab == tab ? "> %s <" : "%s",
        label);
    if (!widget_button(app, rect, text))
        return;
    app->ranking->tab = tab;
    app->ranking->undo_ask = 0;
    scroll_jump(&app->ranking->list, 0);
}

static void tabs(app_t *app)
{
    float names[4] = {650, CAT_BAR_Y, 56, 56};

    tab_button(app, (float [4]){895, CAT_BAR_Y, 200, 56}, TAB_TIERS,
        "Tier list");
    tab_button(app, (float [4]){1110, CAT_BAR_Y, 240, 56}, TAB_BOARD,
        "Leaderboard");
    tab_button(app, (float [4]){1365, CAT_BAR_Y, 205, 56}, TAB_TIES,
        "Départage");
    tab_button(app, (float [4]){1585, CAT_BAR_Y, 195, 56}, TAB_HISTORY,
        "Historique");
    widget_tip(app, names, app->show_images ? "Voir les noms"
        : "Voir les images");
    if (widget_icon(app, names, ICON_IMAGE, app->show_images ? ICON_FILLED
            : ICON_OUTLINE))
        app->show_images = !app->show_images;
}

static void tab_content(app_t *app, ranking_t const *ranking)
{
    if (ranking->tab == TAB_HISTORY) {
        ranking_history(app);
        return;
    }
    if (ranking->nb == 0) {
        ranking_list_frame(app, CONTENT_Y, 0);
        widget_text(app, "Rien à classer dans ce classement.",
            (float [3]){130, CONTENT_Y, 30}, &sfWhite);
        return;
    }
    if (ranking->tab == TAB_TIERS)
        ranking_tiers(app);
    if (ranking->tab == TAB_BOARD)
        ranking_board(app);
    if (ranking->tab == TAB_TIES)
        ranking_ties(app);
}

/*
** each tab gives the size of its list (ranking->list), whose slider is
** drawn on the right
*/
static void content(app_t *app, ranking_t const *ranking)
{
    tab_content(app, ranking);
    scroll_bar_draw(app, &ranking->list);
}

static void bottom(app_t *app, ranking_t *ranking)
{
    float reset[4] = {1480, 840 + app->extra_h, 300, 56};

    if (widget_back(app))
        scene_back(app);
    if (widget_icon_button(app, reset, ICON_UNDO, "Réinitialiser"))
        ranking->reset = 1;
}

/*
** the reset window takes the clicks while it is open, then the list of
** the categories (drawn above the screen)
*/
void ranking_view(app_t *app)
{
    ranking_t *ranking = app->ranking;

    if (ranking->reset && app->wg->mode == WG_CLICK) {
        ranking_reset_view(app);
        return;
    }
    if (app->wg->mode == WG_CLICK && category_overlay(app,
            &app->arena.category, NB_CATEGORIES + 1)) {
        ranking_build(app);
        return;
    }
    widget_text(app, "Classements", (float [3]){130, 115, 40}, &sfWhite);
    ranking_search(app);
    if (board_bar(app, &app->arena))
        ranking_build(app);
    if (category_bar(app, &app->arena.category, 0))
        ranking_build(app);
    tabs(app);
    content(app, ranking);
    bottom(app, ranking);
    if (app->wg->mode == WG_DRAW && category_overlay(app,
            &app->arena.category, NB_CATEGORIES + 1))
        ranking_build(app);
    if (ranking->reset)
        ranking_reset_view(app);
}
