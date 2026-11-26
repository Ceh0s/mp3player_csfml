/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** icons of the atlas drawn as buttons (immediate mode, like the other
** widgets): a single sprite is moved for each of them; an icon can also
** be put on the left of the text of a button
*/

#include "mp3player.h"

/*
** state : ICON_OUTLINE, ICON_FILLED (a state that is on) or ICON_DIM
** (a state that is off); lighter when the mouse is over it
*/
static void draw_icon(app_t *app, float const *rect, int icon, int state)
{
    sfSprite *sprite = app->wg->icon;
    int filled = state == ICON_FILLED;
    sfIntRect area = {ICON_X(icon, filled), ICON_Y(icon, filled),
        ATLAS_CELL, ATLAS_CELL};
    int dark = app->lib->settings.dark;
    sfColor color = {255, 255, 255, state == ICON_DIM ? 110 : 235};
    sfVector2f scale = {rect[2] / ATLAS_CELL, rect[3] / ATLAS_CELL};

    if (dark)
        color.a = state == ICON_DIM ? 90 : 185;
    if (mouse_in_rect(app, rect))
        color = (sfColor){255, 220, 120, 255};
    sfSprite_setTextureRect(sprite, area);
    sfSprite_setScale(sprite, scale);
    sfSprite_setPosition(sprite, (sfVector2f){rect[0], rect[1]});
    sfSprite_setColor(sprite, color);
    sfRenderWindow_drawSprite(app->window, sprite, NULL);
}

/*
** rect = {x, y, w, h}; returns 1 if this icon was clicked
*/
int widget_icon(app_t *app, float const *rect, int icon, int state)
{
    widgets_t *wg = app->wg;

    if (wg->mode == WG_CLICK && mouse_in_rect(app, rect)) {
        wg->mode = WG_IDLE;
        return 1;
    }
    if (wg->mode != WG_DRAW)
        return 0;
    draw_icon(app, rect, icon, state);
    return 0;
}

/*
** an icon alone in place of a text button: its name is shown when the
** mouse is over it; returns 1 if clicked
*/
int widget_icon_tip(app_t *app, float const *rect, int icon,
    char const *tip)
{
    widget_tip(app, rect, tip);
    return widget_icon(app, rect, icon, ICON_OUTLINE);
}

/*
** the back arrow in the top left corner of a screen (same place on every
** screen, Escape does the same); returns 1 if clicked
*/
int widget_back(app_t *app)
{
    return widget_icon_tip(app, (float [4]){BACK_X, 16, 64, 64}, ICON_BACK,
        "Retour");
}

/*
** a button with an icon on the left of its text; returns 1 if clicked
*/
int widget_icon_button(app_t *app, float const *rect, int icon,
    char const *label)
{
    float side = rect[3] - 12;
    float box[4] = {rect[0] + side + 12, rect[1], rect[2] - side - 12,
        rect[3]};

    if (widget_button(app, rect, ""))
        return 1;
    if (app->wg->mode != WG_DRAW)
        return 0;
    draw_icon(app, (float [4]){rect[0] + 8, rect[1] + 6, side, side}, icon,
        ICON_OUTLINE);
    widget_place(app, box);
    text_set(app->wg->text, label);
    sfText_setFillColor(app->wg->text, sfWhite);
    text_draw(app, app->wg->text);
    return 0;
}
