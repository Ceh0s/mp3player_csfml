/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** drawing of a list that scrolls (scroll.c): its rows are cut at the
** top and bottom of its area, a slider is drawn on its right
*/

#include "mp3player.h"

/*
** to call before the rows: in drawing, nothing is drawn out of the area;
** returns 0 when the rows must be skipped (a click out of the area, or
** already taken)
*/
int scroll_rows_begin(app_t *app, scroll_t const *s)
{
    if (app->wg->mode == WG_CLICK)
        return mouse_in_rect(app, s->area);
    if (app->wg->mode != WG_DRAW)
        return 0;
    clip_begin(app, s->area);
    return 1;
}

void scroll_rows_end(app_t *app)
{
    if (app->wg->mode == WG_DRAW)
        clip_end(app);
}

/*
** nothing when the whole content is shown
*/
void scroll_bar_draw(app_t *app, scroll_t const *s)
{
    float handle[4];
    float track[4] = {0, s->area[1], SLIDER_W, s->area[3]};
    sfColor back = {255, 255, 255, 30};
    sfColor idle = {255, 255, 255, 120};
    sfColor lit = {255, 200, 0, 220};

    if (app->wg->mode != WG_DRAW || !scroll_handle(s, handle))
        return;
    track[0] = handle[0];
    widget_rect(app, track, &back);
    widget_rect(app, handle, s->held || mouse_in_rect(app, handle)
        ? &lit : &idle);
}
