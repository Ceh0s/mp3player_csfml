/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** images of a tier of the tier list, in a grid: as big as possible
** (TILE_MAX at most) so that all of them fit in the row; the name is
** shown when the mouse is over an image
*/

#include "mp3player.h"

static int tier_count(app_t *app, int tier)
{
    ranking_t const *ranking = app->ranking;
    int count = 0;

    for (int i = 0; i < ranking->nb; i++)
        count += ranking->tiers[i] == tier
            && ranking_match(app, &ranking->ranks[i]);
    return count;
}

/*
** biggest size (multiple of 4, TILE_MIN at least) for which count
** images fit in the row = {y, h}
*/
static float tile_size(int count, float const *row)
{
    float size = TILE_MAX;
    int per_line = 0;
    int lines = 0;

    while (size > TILE_MIN) {
        per_line = (TILES_RIGHT - TILES_X) / (size + TILE_GAP);
        lines = (count + per_line - 1) / per_line;
        if (lines * (size + TILE_GAP) <= row[1] - 8)
            return size;
        size -= 4;
    }
    return TILE_MIN;
}

static void tile(app_t *app, rank_t const *rank, float const *rect)
{
    widget_image(app, image_get(app, rank, 1), rect, rank->artist != NULL);
    widget_tip(app, rect, rank_name(rank));
    ranking_hover(app, rank, rect);
}

/*
** the area of a tier shown alone, right of its label; it scrolls
** (ranking->list), content = height of its images or names
*/
void ranking_open_frame(app_t *app, float const *row, float content)
{
    float area[4] = {TILES_X - 4, row[0] + 4,
        PANEL_X + PANEL_W - TILES_X + 4, row[1] - 8};

    scroll_frame(&app->ranking->list, area, content);
}

/*
** a tier shown alone: every image, big, scrolled
*/
static void open_tiles(app_t *app, int tier, float const *row)
{
    ranking_t *ranking = app->ranking;
    float step = TILE_MAX + TILE_GAP;
    int per_line = (TILES_RIGHT - TILES_X) / step;
    int lines = (tier_count(app, tier) + per_line - 1) / per_line;
    int shown = 0;
    float rect[4] = {0, 0, TILE_MAX, TILE_MAX};

    ranking_open_frame(app, row, lines * step);
    scroll_rows_begin(app, &ranking->list);
    for (int i = 0; i < ranking->nb; i++) {
        if (ranking->tiers[i] != tier
            || !ranking_match(app, &ranking->ranks[i]))
            continue;
        rect[0] = TILES_X + (shown % per_line) * step;
        rect[1] = scroll_y(&ranking->list, (shown / per_line) * step);
        shown++;
        if (scroll_seen(&ranking->list, rect[1], TILE_MAX))
            tile(app, &ranking->ranks[i], rect);
    }
    scroll_rows_end(app);
}

/*
** row = {y, h}; returns the number of images that do not fit (none for
** a tier shown alone: it scrolls)
*/
int ranking_tiles(app_t *app, int tier, float const *row)
{
    ranking_t *ranking = app->ranking;
    int count = tier_count(app, tier);
    float size = tile_size(count, row);
    int per_line = (TILES_RIGHT - TILES_X) / (size + TILE_GAP);
    int shown = 0;
    int hidden = 0;
    float rect[4] = {0, 0, size, size};

    if (ranking->open_tier >= 0) {
        open_tiles(app, tier, row);
        return 0;
    }
    for (int i = 0; i < ranking->nb; i++) {
        if (ranking->tiers[i] != tier
            || !ranking_match(app, &ranking->ranks[i]))
            continue;
        rect[0] = TILES_X + (shown % per_line) * (size + TILE_GAP);
        rect[1] = row[0] + 4 + (shown / per_line) * (size + TILE_GAP);
        shown++;
        if (rect[1] + size > row[0] + row[1] - 4)
            hidden++;
        else
            tile(app, &ranking->ranks[i], rect);
    }
    return hidden;
}
