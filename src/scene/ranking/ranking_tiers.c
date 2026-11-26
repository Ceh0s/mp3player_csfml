/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** tier list: S/A/B/C/D/F by percentile (10 / 20 / 25 / 20 / 15 / 10 %)
** among those having at least one battle, the others are "Non classé";
** the titles of a tier are written one after the other in its row, or
** its images are shown (ranking_tiles.c)
*/

#include "mp3player.h"

static const int tier_cumul[NB_TIERS] = {10, 30, 55, 75, 90, 100};

static char const *const tier_names[NB_TIERS + 1] = {
    "S", "A", "B", "C", "D", "F", "Non\nclassé"
};

static const sfColor tier_colors[NB_TIERS + 1] = {
    {255, 127, 127, 255},
    {255, 191, 127, 255},
    {255, 223, 127, 255},
    {191, 255, 127, 255},
    {127, 191, 255, 255},
    {191, 127, 255, 255},
    {150, 150, 150, 255}
};

/*
** rank (0 = best) among nb songs: first tier whose cumulated
** percentage is above the position of the song
*/
int ranking_tier(int rank, int nb)
{
    for (int t = 0; t < NB_TIERS; t++)
        if (rank * 100 < tier_cumul[t] * nb)
            return t;
    return NB_TIERS - 1;
}

/*
** tiers[i] : tier of ranks[i], TIER_UNRANKED without any battle
*/
void ranking_assign_tiers(ranking_t *ranking)
{
    int rated = 0;
    int pos = 0;

    for (int i = 0; i < ranking->nb; i++)
        rated += ranking->ranks[i].battles > 0;
    for (int i = 0; i < ranking->nb; i++) {
        ranking->tiers[i] = TIER_UNRANKED;
        if (ranking->ranks[i].battles == 0)
            continue;
        ranking->tiers[i] = ranking_tier(pos, rated);
        pos++;
    }
}

/*
** writes title at pos = {x, y, size} and moves pos after it, going to
** the next line if needed; returns 0 if it does not fit in the row
*/
static int place_title(app_t *app, char const *title, float *pos,
    float const *row)
{
    float width = widget_text_width(app, title, pos[2]) + 40;

    if (pos[0] + width > 1700 && pos[0] > 250) {
        pos[0] = 250;
        pos[1] += 30;
    }
    pos[0] += width;
    if (pos[1] + 30 > row[0] + row[1])
        return 0;
    widget_text(app, title, (float [3]){pos[0] - width, pos[1], pos[2]},
        &sfWhite);
    return 1;
}

/*
** the rect of the title just placed (pos is after it)
*/
static void title_hover(app_t *app, rank_t const *rank, float const *pos,
    float width)
{
    float rect[4] = {pos[0] - width, pos[1], width - 30, 28};

    ranking_hover(app, rank, rect);
}

/*
** place of the next name of a tier shown alone: pos = {x, line} moves
** after it (line = its height from the top of the names); returns its
** width
*/
static float next_name(app_t *app, char const *name, float *pos)
{
    float width = widget_text_width(app, name, 20) + 40;

    if (pos[0] + width > 1700 && pos[0] > 250) {
        pos[0] = 250;
        pos[1] += 30;
    }
    pos[0] += width;
    return width;
}

/*
** height of the names of a tier shown alone
*/
static float names_height(app_t *app, int tier)
{
    ranking_t *ranking = app->ranking;
    float pos[2] = {250, 4};

    for (int i = 0; i < ranking->nb; i++)
        if (ranking->tiers[i] == tier
            && ranking_match(app, &ranking->ranks[i]))
            next_name(app, rank_name(&ranking->ranks[i]), pos);
    return pos[1] + 34;
}

/*
** a tier shown alone: every name, scrolled
*/
static void open_titles(app_t *app, int tier, float const *row)
{
    ranking_t *ranking = app->ranking;
    scroll_t *list = &ranking->list;
    float pos[2] = {250, 4};
    float width = 0;
    float y = 0;

    ranking_open_frame(app, row, names_height(app, tier));
    scroll_rows_begin(app, list);
    for (int i = 0; i < ranking->nb; i++) {
        if (ranking->tiers[i] != tier
            || !ranking_match(app, &ranking->ranks[i]))
            continue;
        width = next_name(app, rank_name(&ranking->ranks[i]), pos);
        y = scroll_y(list, pos[1]);
        if (!scroll_seen(list, y, 30))
            continue;
        widget_text(app, rank_name(&ranking->ranks[i]),
            (float [3]){pos[0] - width, y, 20}, &sfWhite);
        title_hover(app, &ranking->ranks[i], (float [2]){pos[0], y}, width);
    }
    scroll_rows_end(app);
}

/*
** row = {y, h}; the titles that do not fit are counted in "+N" (all of
** them are shown, scrolled, in a tier shown alone)
*/
static int draw_titles(app_t *app, int tier, float const *row)
{
    ranking_t *ranking = app->ranking;
    float pos[3] = {250, row[0] + 8, 20};
    int hidden = 0;
    char const *name = NULL;

    if (ranking->open_tier >= 0) {
        open_titles(app, tier, row);
        return 0;
    }
    for (int i = 0; i < ranking->nb; i++) {
        if (ranking->tiers[i] != tier
            || !ranking_match(app, &ranking->ranks[i]))
            continue;
        name = rank_name(&ranking->ranks[i]);
        if (!place_title(app, name, pos, row))
            hidden++;
        else
            title_hover(app, &ranking->ranks[i], pos,
                widget_text_width(app, name, pos[2]) + 40);
    }
    return hidden;
}

/*
** in dark mode the box of the tier is darker and its letter white (black
** on a light box otherwise)
*/
static void draw_row(app_t *app, int tier, float const *row)
{
    char more[16];
    int hidden = 0;
    float label[4] = {130, row[0], 100, row[1] - 8};
    int dark = app->lib->settings.dark;
    sfColor const *letter = dark ? &sfWhite : &sfBlack;
    sfColor box = tier_colors[tier];

    widget_help(app, label, app->ranking->open_tier < 0
        ? "Voir tout ce tier" : "Revenir à la tier list");
    if (dark) {
        box.r /= 2;
        box.g /= 2;
        box.b /= 2;
    }
    widget_rect(app, label, &box);
    if (tier == TIER_UNRANKED)
        widget_text(app, tier_names[tier],
            (float [3]){142, row[0] + row[1] / 2 - 28, 20}, letter);
    else
        widget_text(app, tier_names[tier],
            (float [3]){165, row[0] + row[1] / 2 - 30, 40}, letter);
    if (app->show_images)
        hidden = ranking_tiles(app, tier, row);
    else
        hidden = draw_titles(app, tier, row);
    if (hidden == 0)
        return;
    snprintf(more, sizeof(more), "+%d", hidden);
    widget_text(app, more, (float [3]){1720, row[0] + 8, 22}, &sfWhite);
}

/*
** a click on the colored label of a tier opens it alone, or closes it
*/
static int label_clicked(app_t *app, float const *row)
{
    float label[4] = {130, row[0], 100, row[1] - 8};

    if (app->wg->mode != WG_CLICK || !mouse_in_rect(app, label))
        return 0;
    app->wg->mode = WG_IDLE;
    return 1;
}

/*
** one tier on the whole table (open_tier), its label closes it
*/
static void open_tier(app_t *app)
{
    ranking_t *ranking = app->ranking;
    float row[2] = {CONTENT_Y, CONTENT_BOTTOM + app->extra_h - CONTENT_Y};

    if (label_clicked(app, row)) {
        ranking->open_tier = -1;
        scroll_jump(&ranking->list, 0);
        return;
    }
    if (app->wg->mode == WG_DRAW)
        draw_row(app, ranking->open_tier, row);
}

/*
** the rows share the height left on the screen
*/
void ranking_tiers(app_t *app)
{
    float height = (CONTENT_BOTTOM + app->extra_h - CONTENT_Y)
        / (NB_TIERS + 1);
    float row[2] = {0, height};

    if (app->ranking->open_tier >= 0) {
        open_tier(app);
        return;
    }
    ranking_list_frame(app, CONTENT_Y, 0);
    for (int t = 0; t <= TIER_UNRANKED; t++) {
        row[0] = CONTENT_Y + t * height;
        if (label_clicked(app, row)) {
            app->ranking->open_tier = t;
            scroll_jump(&app->ranking->list, 0);
            return;
        }
        if (app->wg->mode == WG_DRAW)
            draw_row(app, t, row);
    }
}
