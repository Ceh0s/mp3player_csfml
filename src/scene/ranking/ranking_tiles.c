/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** images of a tier of the tier list, in a grid: as big as possible
** (TILE_MAX at most) so that all of them fit in the row; the name is
** shown when the mouse is over an image
*/

#include "mp3player.h"

static int tier_count(ranking_t const *ranking, int tier)
{
    int count = 0;

    for (int i = 0; i < ranking->nb; i++)
        count += ranking_tier(i, ranking->nb) == tier;
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
}

/*
** row = {y, h}; returns the number of images that do not fit
*/
int ranking_tiles(app_t *app, int tier, float const *row)
{
    ranking_t *ranking = app->ranking;
    float size = tile_size(tier_count(ranking, tier), row);
    int per_line = (TILES_RIGHT - TILES_X) / (size + TILE_GAP);
    int shown = 0;
    int hidden = 0;
    float rect[4] = {0, 0, size, size};

    for (int i = 0; i < ranking->nb; i++) {
        if (ranking_tier(i, ranking->nb) != tier)
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
