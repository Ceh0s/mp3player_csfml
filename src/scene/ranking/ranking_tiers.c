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
** row = {y, h}; the titles that do not fit are counted in "+N"
*/
static int draw_titles(app_t *app, int tier, float const *row)
{
    ranking_t *ranking = app->ranking;
    float pos[3] = {250, row[0] + 8, 20};
    int hidden = 0;

    for (int i = 0; i < ranking->nb; i++) {
        if (ranking->tiers[i] != tier)
            continue;
        if (!place_title(app, rank_name(&ranking->ranks[i]), pos, row))
            hidden++;
    }
    return hidden;
}

static void draw_row(app_t *app, int tier, float const *row)
{
    char more[16];
    int hidden = 0;

    widget_rect(app, (float [4]){130, row[0], 100, row[1] - 8},
        &tier_colors[tier]);
    if (tier == TIER_UNRANKED)
        widget_text(app, tier_names[tier],
            (float [3]){142, row[0] + row[1] / 2 - 28, 20}, &sfBlack);
    else
        widget_text(app, tier_names[tier],
            (float [3]){165, row[0] + row[1] / 2 - 30, 40}, &sfBlack);
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
** the rows share the height left on the screen
*/
void ranking_tiers(app_t *app)
{
    float height = (CONTENT_BOTTOM + app->extra_h - CONTENT_Y)
        / (NB_TIERS + 1);

    if (app->wg->mode != WG_DRAW)
        return;
    for (int t = 0; t <= TIER_UNRANKED; t++)
        draw_row(app, t, (float [2]){CONTENT_Y + t * height, height});
}
