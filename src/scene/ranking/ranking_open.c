/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a right click on a song of the rankings (tier list, leaderboard or
** "Départage") opens its edit page, on an artist its page; the screen
** is shown again as it was when coming back (app->ranking_keep)
*/

#include "mp3player.h"

/*
** called while drawing each song or artist: the one under the mouse is
** kept for the next right click
*/
void ranking_hover(app_t *app, rank_t const *rank, float const *rect)
{
    if (app->wg->mode != WG_DRAW || !mouse_in_rect(app, rect))
        return;
    app->ranking->hover = *rank;
}

void ranking_open_hover(app_t *app)
{
    ranking_t *ranking = app->ranking;
    ranking_keep_t keep = {1, ranking->tab, ranking->scroll,
        ranking->open_tier};

    if (!ranking->hover.song && !ranking->hover.artist)
        return;
    app->ranking_keep = keep;
    page_open(app, &ranking->hover);
}

/*
** in ranking_init, after the ranking is built
*/
void ranking_restore(app_t *app)
{
    ranking_t *ranking = app->ranking;

    if (!app->ranking_keep.active)
        return;
    ranking->tab = app->ranking_keep.tab;
    ranking->scroll = app->ranking_keep.scroll;
    ranking->open_tier = app->ranking_keep.open_tier;
    app->ranking_keep.active = 0;
    if (ranking->tab != TAB_TIERS)
        ranking_scroll(app, 0);
}
