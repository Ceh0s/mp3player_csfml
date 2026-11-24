/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** text field: a click puts the cursor on the character under the
** mouse, the field with the focus draws its cursor and scrolls so that
** it stays visible (a long text goes past the box otherwise)
*/

#include "mp3player.h"

/*
** the text of a field starts 14 px after the box, centered in height
*/
void widget_place(app_t *app, float const *rect)
{
    float size = rect[3] * 0.5;
    sfVector2f pos = {rect[0] + 14, rect[1] + (rect[3] - size) / 2 - 4};

    sfText_setCharacterSize(app->wg->text, size);
    sfText_setPosition(app->wg->text, pos);
}

/*
** at most FIELD_SHOWN bytes of src, never half a character
*/
static void copy_part(char *part, char const *src)
{
    size_t len = 0;
    size_t next = 0;

    while (src[len] != '\0') {
        next = utf8_next(src, len);
        if (next > FIELD_SHOWN)
            break;
        len = next;
    }
    memcpy(part, src, len);
    part[len] = '\0';
}

/*
** the text from the byte first goes in the shared sfText; returns the
** x of the character index (counted from first)
*/
static float set_part(app_t *app, char const *value, size_t first,
    int index)
{
    char part[FIELD_SHOWN + 1];

    copy_part(part, value + first);
    text_set(app->wg->text, part);
    return text_char_x(app, app->wg->text, index);
}

/*
** the first byte shown moves on until the cursor fits in the box
*/
static size_t scroll(app_t *app, float const *rect, caret_t *caret)
{
    float right = rect[0] + rect[2] - 14;
    int index = utf8_count(caret->buf + caret->first,
        caret->pos - caret->first);
    float x = 0;

    while (caret->first < caret->pos) {
        x = set_part(app, caret->buf, caret->first, index);
        if (caret->pos - caret->first <= FIELD_SHOWN && x <= right)
            break;
        caret->first = utf8_next(caret->buf, caret->first);
        index--;
    }
    return caret->first;
}

/*
** the cursor goes on the character nearest the mouse (the middle of
** each character is compared to it)
*/
static void click(app_t *app, float const *rect, char *value, int focused)
{
    caret_t *caret = caret_of(app, value);
    sfText *text = app->wg->text;
    size_t pos = 0;
    float middle = 0;

    if (!focused)
        caret->first = 0;
    pos = caret->first;
    widget_place(app, rect);
    set_part(app, value, caret->first, 0);
    for (int i = 0; value[pos] != '\0' && pos - caret->first < FIELD_SHOWN;
        i++) {
        middle = (text_char_x(app, text, i)
            + text_char_x(app, text, i + 1)) / 2;
        if (middle >= app->mouse.x)
            break;
        pos = utf8_next(value, pos);
    }
    caret->pos = pos;
}

/*
** only the characters that end inside the box are drawn
*/
static void draw_clipped(app_t *app, char const *str, float const *rect)
{
    char part[FIELD_SHOWN + 1];
    float right = rect[0] + rect[2] - 14;
    size_t end = 0;

    copy_part(part, str);
    text_set(app->wg->text, part);
    for (int i = 1; part[end] != '\0'
        && text_char_x(app, app->wg->text, i) <= right; i++)
        end = utf8_next(part, end);
    part[end] = '\0';
    text_set(app->wg->text, part);
    sfText_setFillColor(app->wg->text, sfWhite);
    text_draw(app, app->wg->text);
}

static void draw_caret(app_t *app, float const *rect, float x)
{
    sfRectangleShape *box = app->wg->box;
    sfVector2f pos = {x, rect[1] + rect[3] * 0.2};

    sfRectangleShape_setPosition(box, pos);
    sfRectangleShape_setSize(box, (sfVector2f){2, rect[3] * 0.6});
    sfRectangleShape_setFillColor(box, sfWhite);
    sfRectangleShape_setOutlineThickness(box, 0);
    sfRenderWindow_drawRectangleShape(app->window, box, NULL);
}

/*
** the field with the focus: the first byte shown, and in x where its
** cursor goes
*/
static size_t focused_part(app_t *app, float const *rect, char *value,
    float *x)
{
    caret_t *caret = caret_of(app, value);
    size_t first = scroll(app, rect, caret);
    int index = utf8_count(value + first, caret->pos - first);

    caret->shown = 1;
    *x = set_part(app, value, first, index);
    return first;
}

/*
** rect = {x, y, w, h}; returns 1 if this field was clicked; the text
** after the right of the box is not drawn
*/
int widget_field(app_t *app, float const *rect, char *value, int focused)
{
    size_t first = 0;
    float x = 0;

    if (app->wg->mode == WG_CLICK && mouse_in_rect(app, rect)) {
        click(app, rect, value, focused);
        app->wg->mode = WG_IDLE;
        return 1;
    }
    if (app->wg->mode != WG_DRAW)
        return 0;
    widget_box(app, rect, focused);
    if (focused)
        first = focused_part(app, rect, value, &x);
    draw_clipped(app, value + first, rect);
    if (focused)
        draw_caret(app, rect, x);
    return 0;
}
