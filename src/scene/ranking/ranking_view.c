/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the rankings screen: category, tabs, buttons
** (called to draw it and to handle a click, see widgets.c)
*/

#include "mp3player.h"

/*
** tier list or leaderboard, images or names in the tier list
*/
static void tabs(app_t *app, ranking_t *ranking)
{
    ranking_tab_t tab = ranking->tab;

    if (widget_button(app, (float [4]){1030, CAT_BAR_Y, 220, 56},
            tab == TAB_TIERS ? "> Tier list <" : "Tier list"))
        ranking->tab = TAB_TIERS;
    if (widget_button(app, (float [4]){1270, CAT_BAR_Y, 250, 56},
            tab == TAB_BOARD ? "> Leaderboard <" : "Leaderboard"))
        ranking->tab = TAB_BOARD;
    if (widget_button(app, (float [4]){1540, CAT_BAR_Y, 240, 56},
            tab == TAB_TIES ? "> Départage <" : "Départage"))
        ranking->tab = TAB_TIES;
    if (widget_button(app, (float [4]){700, CAT_BAR_Y, 300, 56},
            app->show_images ? "Afficher les noms" : "Afficher les images"))
        app->show_images = !app->show_images;
}

static void content(app_t *app, ranking_t const *ranking)
{
    if (ranking->nb == 0) {
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

static void bottom(app_t *app, ranking_t *ranking)
{
    float reset[4] = {1480, 840 + app->extra_h, 300, 56};

    if (widget_button(app, (float [4]){130, 840 + app->extra_h, 260, 56},
            "Retour"))
        scene_back(app);
    widget_help(app, reset, "Remet les Elo à 1500 (ce classement ou tout) ; "
        "l'historique reste dans battles.log");
    if (widget_button(app, reset, "Réinitialiser"))
        ranking->reset = 1;
}

/*
** the reset window takes the clicks while it is open
*/
void ranking_view(app_t *app)
{
    ranking_t *ranking = app->ranking;

    if (ranking->reset && app->wg->mode == WG_CLICK) {
        ranking_reset_view(app);
        return;
    }
    widget_text(app, "Classements", (float [3]){130, 115, 40}, &sfWhite);
    ranking_search(app);
    if (board_bar(app, &app->arena))
        ranking_build(app);
    if (category_bar(app, &app->arena.category, NB_CATEGORIES + 1, 0))
        ranking_build(app);
    tabs(app, ranking);
    content(app, ranking);
    bottom(app, ranking);
    if (ranking->reset)
        ranking_reset_view(app);
}
