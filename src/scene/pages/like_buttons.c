/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** "J'aime" / "Je n'aime pas" of a song or an artist, shared by the
** library bar, the song rows, the edit screen, the artist page and the
** battle cards: a heart and a crossed heart, filled when chosen; a
** second click on the chosen one removes it
*/

#include "mp3player.h"

/*
** chosen : 1 (like) or -1 (dislike); returns 1 (the automatic playlists
** are then made again)
*/
static int like_change(app_t *app, elo_kind_t kind, int id, int chosen)
{
    like_set(app->lib, kind, id, chosen);
    player_resync(app);
    return 1;
}

/*
** pos = {x, y, vertical (1: one under the other)}; returns 1 if the
** like changed
*/
int like_buttons(app_t *app, elo_kind_t kind, int id, float const *pos)
{
    int value = like_get(app->lib, kind, id);
    float like[4] = {pos[0], pos[1], 52, 52};
    float dislike[4] = {pos[0] + 62, pos[1], 52, 52};

    if (pos[2] != 0) {
        dislike[0] = pos[0];
        dislike[1] = pos[1] + 62;
    }
    widget_help(app, like, value > 0 ? "J'aime (cliquer pour retirer)"
        : "J'aime");
    widget_help(app, dislike, value < 0 ? "Je n'aime pas (cliquer pour "
        "retirer)" : "Je n'aime pas");
    if (widget_icon(app, like, ICON_LIKE, value > 0 ? ICON_FILLED
            : ICON_OUTLINE))
        return like_change(app, kind, id, 1);
    if (widget_icon(app, dislike, ICON_DISLIKE, value < 0 ? ICON_FILLED
            : ICON_OUTLINE))
        return like_change(app, kind, id, -1);
    return 0;
}

/*
** small icon of a song row: the like or dislike given is always shown,
** both are shown while the mouse is over the row (rect: the row); the
** icons are placed at x; returns 1 if the like changed
*/
int like_row(app_t *app, song_t const *song, float const *rect, float x)
{
    int value = like_get(app->lib, KIND_SONG, song->id);
    float size = rect[3] - 16;
    float like[4] = {x, rect[1] + 8, size, size};
    float dislike[4] = {x + size + 8, rect[1] + 8, size, size};
    int over = mouse_in_rect(app, rect);

    if ((over || value > 0) && widget_icon(app, like, ICON_LIKE,
            value > 0 ? ICON_FILLED : ICON_DIM))
        return like_change(app, KIND_SONG, song->id, 1);
    if ((over || value < 0) && widget_icon(app, dislike, ICON_DISLIKE,
            value < 0 ? ICON_FILLED : ICON_DIM))
        return like_change(app, KIND_SONG, song->id, -1);
    return 0;
}
