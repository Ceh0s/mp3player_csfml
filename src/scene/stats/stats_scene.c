/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** statistics screen: a leaderboard of the listening time of the songs
** or the artists, for every song or a playlist, between two dates or
** over all the history, with the Elo of a category next to it if asked
** (drawn with the widgets, like the rankings)
*/

#include "mp3player.h"

int stats_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};

    app->stats = calloc(1, sizeof(statscene_t));
    if (!app->stats)
        return 84;
    app->stats->bg = background_create(BG_BLUR_PATH,
        &app->stats->bg_texture);
    app->stats->panel = rect_create(panel, &dark);
    if (!app->stats->bg || !app->stats->panel)
        return 84;
    app->stats->nb_log = stats_log_load(&app->stats->log);
    app->stats->dirty = 1;
    return 0;
}

/*
** the date fields take digits and '-' (8 = backspace, 9 = tab: the
** other field, 13 = enter: the keys go back to the screen)
*/
static void date_text(app_t *app, sfUint32 code)
{
    statscene_t *scene = app->stats;
    stats_opt_t *opt = &app->stats_opt;
    char *buf = scene->focus == 1 ? opt->from : opt->to;

    if (code == 9)
        scene->focus = scene->focus == 1 ? 2 : 1;
    if (code == 13)
        scene->focus = 0;
    if (code != 8 && code != '-' && (code < '0' || code > '9'))
        return;
    if (text_input_n(app, buf, STAT_DATE_LEN, code))
        scene->dirty = 1;
}

/*
** escape gives the keys back to the screen first, then leaves it
*/
static void escape(app_t *app)
{
    if (app->stats->focus)
        app->stats->focus = 0;
    else
        scene_back(app);
}

/*
** a click outside the date fields takes the keys back from them
*/
void stats_event(app_t *app, sfEvent *event)
{
    if (event->type == sfEvtTextEntered && app->stats->focus)
        date_text(app, event->text.unicode);
    if (event->type == sfEvtMouseWheelScrolled)
        stats_scroll(app, event->mouseWheelScroll.delta);
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyEscape)
        escape(app);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->stats->focus = 0;
    app->wg->mode = WG_CLICK;
    stats_view(app);
    app->wg->mode = WG_DRAW;
}

void stats_draw(app_t *app)
{
    if (app->stats->dirty)
        stats_build(app);
    sfRenderWindow_drawSprite(app->window, app->stats->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->stats->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    stats_view(app);
    widget_tip_draw(app);
}

void stats_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->stats->bg);
    sfRectangleShape_setSize(app->stats->panel, size);
    stats_scroll(app, 0);
}

void stats_destroy(app_t *app)
{
    if (!app->stats)
        return;
    if (app->stats->bg)
        sfSprite_destroy(app->stats->bg);
    if (app->stats->bg_texture)
        sfTexture_destroy(app->stats->bg_texture);
    if (app->stats->panel)
        sfRectangleShape_destroy(app->stats->panel);
    free(app->stats->log);
    free(app->stats->rows);
    free(app->stats);
    app->stats = NULL;
}
