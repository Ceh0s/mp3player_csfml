/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scroll of a list in pixels (library, rankings, statistics...): the
** wheel and the slider move the place aimed at (to), the one shown (pos)
** slides to it; drawn faster (TICK_SMOOTH) while it moves. The list
** gives its area and the height of its content at each drawing.
*/

#include "mp3player.h"

#include <math.h>

static float scroll_max(scroll_t const *s)
{
    float hidden = s->content - s->area[3];

    return hidden > 0 ? hidden : 0;
}

static float bounded(float value, float max)
{
    if (value > max)
        value = max;
    return value < 0 ? 0 : value;
}

/*
** area = {x, y, w, h} of the rows on the screen, content = height of all
** of them; the place is kept valid
*/
void scroll_frame(scroll_t *s, float const *area, float content)
{
    float max = 0;

    memcpy(s->area, area, sizeof(s->area));
    s->content = content;
    max = scroll_max(s);
    s->to = bounded(s->to, max);
    s->pos = bounded(s->pos, max);
}

/*
** pixels > 0 goes down (the wheel gives -delta * 2 rows)
*/
void scroll_by(scroll_t *s, float pixels)
{
    s->to = bounded(s->to + pixels, scroll_max(s));
}

/*
** goes there at once (top of a new list, place kept when coming back);
** made valid by the next scroll_frame
*/
void scroll_jump(scroll_t *s, float pixels)
{
    s->to = pixels < 0 ? 0 : pixels;
    s->pos = s->to;
}

/*
** y on the screen of what is offset pixels below the top of the content
*/
float scroll_y(scroll_t const *s, float offset)
{
    return s->area[1] + offset - (int)s->pos;
}

/*
** 1 if something from y, h high, is at least partly in the area
*/
int scroll_seen(scroll_t const *s, float y, float h)
{
    return y + h > s->area[1] && y < s->area[1] + s->area[3];
}

/*
** handle of the slider; returns 0 if the whole content is shown
*/
int scroll_handle(scroll_t const *s, float *rect)
{
    float max = scroll_max(s);
    float height = s->area[3];

    if (max <= 0)
        return 0;
    rect[0] = s->area[0] + s->area[2] + 14;
    rect[2] = SLIDER_W;
    rect[3] = height * height / s->content;
    if (rect[3] < SLIDER_MIN)
        rect[3] = SLIDER_MIN;
    rect[1] = s->area[1] + (height - rect[3]) * s->pos / max;
    return 1;
}

/*
** a press starting on the slider holds it: the handle where it was
** pressed, its middle brought under the mouse elsewhere on the track
*/
void scroll_grab(app_t *app, scroll_t *s)
{
    int down = sfMouse_isButtonPressed(sfMouseLeft);
    float handle[4];
    float track[4] = {0, s->area[1], SLIDER_W + 16, s->area[3]};

    if (down && !s->was_down && scroll_handle(s, handle)) {
        track[0] = handle[0] - 8;
        if (mouse_in_rect(app, track)) {
            s->held = 1;
            s->grab = app->mouse.y - handle[1];
            if (s->grab < 0 || s->grab > handle[3])
                s->grab = handle[3] / 2;
        }
    }
    s->was_down = down;
}

static void slider_move(app_t *app, scroll_t *s)
{
    float handle[4];
    float room = 0;

    if (!sfMouse_isButtonPressed(sfMouseLeft) || !scroll_handle(s, handle)) {
        s->held = 0;
        return;
    }
    room = s->area[3] - handle[3];
    if (room <= 0)
        return;
    s->to = bounded((app->mouse.y - s->grab - s->area[1]) / room
        * scroll_max(s), scroll_max(s));
    s->pos = s->to;
}

/*
** a part of the distance left is done at each tick, the same in a given
** time whatever the length of the tick; returns 1 while it moves (the
** release of a held slider is then not a click: app->scroll_held)
*/
int scroll_update(app_t *app, scroll_t *s)
{
    float left = 0;

    if (s->held)
        slider_move(app, s);
    if (s->held)
        app->scroll_held = 1;
    left = s->to - s->pos;
    s->pos += left * (1 - exp(-app->delta_us / SCROLL_US));
    if (fabs(s->to - s->pos) < 1)
        s->pos = s->to;
    if (!s->held && s->pos == s->to)
        return 0;
    app->tick_loop = TICK_SMOOTH;
    return 1;
}
