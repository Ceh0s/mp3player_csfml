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
    int tiers = ranking->tab == TAB_TIERS;

    if (widget_button(app, (float [4]){1240, CAT_BAR_Y, 260, 56},
            tiers ? "> Tier list <" : "Tier list"))
        ranking->tab = TAB_TIERS;
    if (widget_button(app, (float [4]){1520, CAT_BAR_Y, 260, 56},
            tiers ? "Leaderboard" : "> Leaderboard <"))
        ranking->tab = TAB_BOARD;
    if (widget_button(app, (float [4]){700, CAT_BAR_Y, 300, 56},
            app->show_images ? "Afficher les noms" : "Afficher les images"))
        app->show_images = !app->show_images;
}

void ranking_view(app_t *app)
{
    ranking_t *ranking = app->ranking;

    widget_text(app, "Classements", (float [3]){130, 115, 40}, &sfWhite);
    if (board_bar(app, &app->arena))
        ranking_build(app);
    if (category_bar(app, &app->arena.category, NB_CATEGORIES + 1, 0))
        ranking_build(app);
    tabs(app, ranking);
    if (ranking->nb == 0)
        widget_text(app, "Aucune battle dans cette catégorie pour "
            "l'instant.", (float [3]){130, CONTENT_Y, 30}, &sfWhite);
    else if (ranking->tab == TAB_TIERS)
        ranking_tiers(app);
    else
        ranking_board(app);
    if (widget_button(app, (float [4]){130, 840 + app->extra_h, 260, 56},
            "Retour"))
        scene_back(app);
}
