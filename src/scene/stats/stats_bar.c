/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** choices of the statistics screen (called to draw them and to handle
** a click): songs or artists, playlist, Elo column, order, then the
** period (two dates) and the search
*/

#include "mp3player.h"

/*
** the playlist after (step = 1) or before (-1) the one shown, 0 being
** "Général" (every present song)
*/
static void next_scope(app_t *app, int step)
{
    int ids[65] = {0};
    int count = 1;
    int at = 0;

    for (playlist_t *pl = app->lib->playlists; pl && count < 65;
        pl = pl->next) {
        at = pl->id == app->stats_opt.playlist_id ? count : at;
        ids[count] = pl->id;
        count++;
    }
    app->stats_opt.playlist_id = ids[(at + step + count) % count];
    app->stats->dirty = 1;
}

static void scope(app_t *app, float y)
{
    playlist_t *pl = playlist_by_id(app->lib, app->stats_opt.playlist_id);
    char name[64];

    if (widget_button(app, (float [4]){370, y, 52, 52}, "<"))
        next_scope(app, -1);
    utf8_cut(name, pl ? pl->name : "Général", 22);
    widget_text(app, name, (float [3]){440, y + 12, 26}, &sfWhite);
    if (widget_button(app, (float [4]){770, y, 52, 52}, ">"))
        next_scope(app, 1);
}

static void elo_button(app_t *app, float y)
{
    stats_opt_t *opt = &app->stats_opt;
    float rect[4] = {840, y, 330, 52};
    char label[64];

    snprintf(label, sizeof(label), "Elo : %s", opt->elo == 0 ? "aucun"
        : category_name(app->lib, opt->elo - 1));
    widget_help(app, rect, "Elo du classement Général");
    if (!widget_button(app, rect, label))
        return;
    if (opt->elo == CAT_TOTAL + 1)
        opt->elo = 0;
    else if (opt->elo == app->lib->nb_cats)
        opt->elo = CAT_TOTAL + 1;
    else
        opt->elo++;
    app->stats->dirty = 1;
}

static void sort_buttons(app_t *app, float y)
{
    char const *sorts[NB_STAT_SORTS] = {"Tri : temps d'écoute",
        "Tri : écoutes", "Tri : skips", "Tri : Elo", "Tri : nom"};
    stats_opt_t *opt = &app->stats_opt;

    if (widget_button(app, (float [4]){1190, y, 330, 52}, sorts[opt->sort])) {
        opt->sort = (opt->sort + 1) % NB_STAT_SORTS;
        if (opt->sort == STAT_SORT_ELO && opt->elo == 0)
            opt->sort++;
        app->stats->dirty = 1;
    }
    if (widget_icon_tip(app, (float [4]){1540, y, 52, 52}, opt->ascending
            ? ICON_ASC : ICON_DESC, opt->ascending ? "Croissant"
            : "Décroissant")) {
        opt->ascending = !opt->ascending;
        app->stats->dirty = 1;
    }
}

void stats_bar(app_t *app)
{
    stats_opt_t *opt = &app->stats_opt;
    float y = 180;

    widget_text(app, "Statistiques d'écoute", (float [3]){130, 115, 40},
        &sfWhite);
    if (widget_button(app, (float [4]){130, y, 220, 52}, opt->artists
            ? "Artistes" : "Titres")) {
        opt->artists = !opt->artists;
        app->stats->dirty = 1;
    }
    scope(app, y);
    elo_button(app, y);
    sort_buttons(app, y);
}
