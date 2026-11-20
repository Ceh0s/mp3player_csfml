/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** simple widgets (texts, buttons, text fields) drawn in immediate mode:
** a single sfText and a single rectangle are reused for everything
*/

#include "mp3player.h"

widgets_t *widgets_create(app_t *app)
{
    widgets_t *wg = calloc(1, sizeof(widgets_t));

    if (!wg)
        return NULL;
    wg->text = text_create(app, 24, 0, 0);
    wg->box = sfRectangleShape_create();
    wg->picture = sfRectangleShape_create();
    wg->circle = sfCircleShape_create();
    if (!wg->text || !wg->box || !wg->picture || !wg->circle) {
        widgets_destroy(wg);
        return NULL;
    }
    sfCircleShape_setPointCount(wg->circle, 64);
    return wg;
}

void widgets_destroy(widgets_t *wg)
{
    if (!wg)
        return;
    if (wg->text)
        sfText_destroy(wg->text);
    if (wg->box)
        sfRectangleShape_destroy(wg->box);
    if (wg->picture)
        sfRectangleShape_destroy(wg->picture);
    if (wg->circle)
        sfCircleShape_destroy(wg->circle);
    free(wg);
}

/*
** pos = {x, y, character size}
*/
void widget_text(app_t *app, char const *str, float const *pos,
    sfColor const *color)
{
    widgets_t *wg = app->wg;

    if (wg->mode != WG_DRAW)
        return;
    text_set(wg->text, str);
    sfText_setCharacterSize(wg->text, pos[2]);
    sfText_setFillColor(wg->text, *color);
    sfText_setPosition(wg->text, (sfVector2f){pos[0], pos[1]});
    sfRenderWindow_drawText(app->window, wg->text, NULL);
}

static void draw_box(app_t *app, float const *rect, int focused)
{
    sfRectangleShape *box = app->wg->box;
    sfColor idle = {25, 25, 25, 210};
    sfColor hover = {70, 70, 70, 230};
    sfColor outline = {255, 200, 0, 255};
    float size = rect[3] * 0.5;
    float text[3] = {rect[0] + 14, rect[1] + (rect[3] - size) / 2 - 4,
        size};

    sfRectangleShape_setPosition(box, (sfVector2f){rect[0], rect[1]});
    sfRectangleShape_setSize(box, (sfVector2f){rect[2], rect[3]});
    sfRectangleShape_setFillColor(box,
        mouse_in_rect(app, rect) ? hover : idle);
    sfRectangleShape_setOutlineThickness(box, focused ? 2 : 0);
    sfRectangleShape_setOutlineColor(box, outline);
    sfRenderWindow_drawRectangleShape(app->window, box, NULL);
    sfText_setCharacterSize(app->wg->text, text[2]);
    sfText_setPosition(app->wg->text, (sfVector2f){text[0], text[1]});
}

/*
** rect = {x, y, w, h}; returns 1 if this button was clicked
*/
int widget_button(app_t *app, float const *rect, char const *label)
{
    widgets_t *wg = app->wg;

    if (wg->mode == WG_CLICK && mouse_in_rect(app, rect)) {
        wg->mode = WG_IDLE;
        return 1;
    }
    if (wg->mode != WG_DRAW)
        return 0;
    draw_box(app, rect, 0);
    text_set(wg->text, label);
    sfText_setFillColor(wg->text, sfWhite);
    sfRenderWindow_drawText(app->window, wg->text, NULL);
    return 0;
}

int widget_field(app_t *app, float const *rect, char const *value,
    int focused)
{
    widgets_t *wg = app->wg;
    char shown[NAME_LEN + 2];

    if (wg->mode == WG_CLICK && mouse_in_rect(app, rect)) {
        wg->mode = WG_IDLE;
        return 1;
    }
    if (wg->mode != WG_DRAW)
        return 0;
    draw_box(app, rect, focused);
    snprintf(shown, sizeof(shown), "%s%s", value, focused ? "|" : "");
    text_set(wg->text, shown);
    sfText_setFillColor(wg->text, sfWhite);
    sfRenderWindow_drawText(app->window, wg->text, NULL);
    return 0;
}
