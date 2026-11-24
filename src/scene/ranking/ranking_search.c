/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** search of the rankings screen: the tabs only show the songs
** (title or artists) or the artists (name) matching the search field;
** the leaderboard keeps their real rank
*/

#include "mp3player.h"

int ranking_match(app_t *app, rank_t const *rank)
{
    if (rank->song)
        return search_song(app, rank->song);
    return search_text(app, rank_name(rank));
}

/*
** number of ranks shown (the same for ranks and order)
*/
int ranking_count(app_t *app)
{
    ranking_t *ranking = app->ranking;
    int nb = 0;

    if (ranking->tab == TAB_HISTORY)
        return ranking_history_count(app);
    for (int i = 0; i < ranking->nb; i++)
        nb += ranking_match(app, &ranking->ranks[i]);
    return nb;
}

/*
** the field is above the tabs; a new search goes back to the top
*/
void ranking_search(app_t *app)
{
    if (app->search.changed) {
        app->search.changed = 0;
        app->ranking->scroll = 0;
    }
    if (!app->ranking->reset)
        search_field(app, (float [4]){1280, 115, 500, 52});
}
