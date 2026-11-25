/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** period of the statistics screen: "Du" / "au" (AAAA-MM-JJ, empty = no
** limit); with a date, only the sessions of listens.log count (the only
** dated ones, kept since the date written next to the fields), without
** any, all the history counts; and the search field
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor orange = {255, 160, 60, 255};

static void date_field(app_t *app, float x, char *buf, int focus)
{
    statscene_t *scene = app->stats;
    float rect[4] = {x, 250, 220, 52};

    widget_help(app, rect, "AAAA-MM-JJ");
    if (widget_field(app, rect, buf, scene->focus == focus))
        scene->focus = focus;
}

/*
** where the dated listening starts, or why the dates are not used
*/
static void note(app_t *app)
{
    stats_opt_t const *opt = &app->stats_opt;
    statscene_t const *scene = app->stats;
    char text[128];
    time_t first = scene->nb_log > 0 ? scene->log[0].date : 0;
    struct tm *day = localtime(&first);

    if (stat_date(opt->from, 0) < 0 || stat_date(opt->to, 1) < 0) {
        widget_text(app, "Date invalide : AAAA-MM-JJ",
            (float [3]){720, 262, 22}, &orange);
        return;
    }
    if (scene->nb_log == 0 || !day)
        snprintf(text, sizeof(text), "Aucune écoute datée");
    else
        snprintf(text, sizeof(text), "Écoutes datées depuis le "
            "%02d/%02d/%d", day->tm_mday, day->tm_mon + 1,
            day->tm_year + 1900);
    widget_text(app, text, (float [3]){720, 262, 22}, &gray);
}

void stats_dates(app_t *app)
{
    stats_opt_t *opt = &app->stats_opt;

    widget_text(app, "Du", (float [3]){130, 262, 24}, &gray);
    date_field(app, 180, opt->from, 1);
    widget_text(app, "au", (float [3]){420, 262, 24}, &gray);
    date_field(app, 465, opt->to, 2);
    note(app);
    if (app->search.changed) {
        app->search.changed = 0;
        app->stats->scroll = 0;
    }
    search_field(app, (float [4]){1300, 250, 480, 52});
}
