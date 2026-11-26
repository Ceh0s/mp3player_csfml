/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** fallback fonts: a character missing from assets/font.ttf (Japanese,
** Thai, Cyrillic, "⧸", "–"...) is drawn with the first fallback font that
** has it (app->fallback: Noto Sans JP, then FreeSerif), the others keep
** the main font. A text without such a character is measured and drawn
** by its sfText as before; a mixed one is cut in runs of characters of
** the same font, placed one after the other.
*/

#include "mp3player.h"

/*
** a character no font has: its glyph is the ".notdef" one of the font
*/
#define NO_CHAR 0x10FFFF

static int same_glyph(sfGlyph const *a, sfGlyph const *b)
{
    return a->advance == b->advance
        && a->bounds.left == b->bounds.left
        && a->bounds.top == b->bounds.top
        && a->bounds.width == b->bounds.width
        && a->bounds.height == b->bounds.height;
}

/*
** CSFML has no hasGlyph: a missing character gets the glyph of NO_CHAR
*/
static int has_glyph(sfFont const *font, sfUint32 c, unsigned int size)
{
    sfGlyph glyph = sfFont_getGlyph(font, c, size, sfFalse, 0);
    sfGlyph none = sfFont_getGlyph(font, NO_CHAR, size, sfFalse, 0);

    return !same_glyph(&glyph, &none);
}

/*
** the font drawing c: the main font if it has it (always for ASCII),
** else the first fallback font that has it, else the main font
*/
sfFont *text_font(app_t *app, sfUint32 c, unsigned int size)
{
    if (c < 128 || has_glyph(app->font, c, size))
        return app->font;
    for (int i = 0; i < NB_FALLBACK; i++)
        if (app->fallback[i] && has_glyph(app->fallback[i], c, size))
            return app->fallback[i];
    return app->font;
}

/*
** 1 if a character of the text needs a fallback font
*/
int text_mixed(app_t *app, sfText const *text)
{
    sfUint32 const *str = sfText_getUnicodeString(text);
    unsigned int size = sfText_getCharacterSize(text);

    for (int i = 0; str[i] != 0; i++)
        if (text_font(app, str[i], size) != app->font)
            return 1;
    return 0;
}

/*
** what the character moves the next one by (as sfText does it)
*/
static float advance(sfFont const *font, sfUint32 c, unsigned int size)
{
    sfUint32 glyph_of = c == '\t' ? ' ' : c;
    float width = sfFont_getGlyph(font, glyph_of, size, sfFalse, 0).advance;

    return c == '\t' ? width * 4 : width;
}

/*
** x of the character index of the text, as sfText_findCharacterPos
** (one line, no scale nor origin)
*/
float text_char_x(app_t *app, sfText const *text, size_t index)
{
    sfUint32 const *str = sfText_getUnicodeString(text);
    unsigned int size = sfText_getCharacterSize(text);
    float x = sfText_getPosition(text).x;
    sfFont *font = NULL;
    sfFont *prev = NULL;

    if (!text_mixed(app, text))
        return sfText_findCharacterPos(text, index).x;
    for (size_t i = 0; i < index && str[i] != 0; i++) {
        font = text_font(app, str[i], size);
        if (i > 0 && font == prev)
            x += sfFont_getKerning(font, str[i - 1], str[i], size);
        x += advance(font, str[i], size);
        prev = font;
    }
    return x;
}

float text_width(app_t *app, sfText const *text)
{
    sfUint32 const *str = sfText_getUnicodeString(text);
    size_t len = 0;

    if (!text_mixed(app, text))
        return sfText_getLocalBounds(text).width;
    while (str[len] != 0)
        len++;
    return text_char_x(app, text, len) - sfText_getPosition(text).x;
}

/*
** the characters from start to end (excluded) drawn with font
*/
static void draw_run(app_t *app, sfText const *text, size_t const *range,
    sfFont const *font)
{
    sfUint32 const *str = sfText_getUnicodeString(text);
    sfUint32 *part = malloc(sizeof(sfUint32) * (range[1] - range[0] + 1));
    sfText *run = app->run_text;
    sfVector2f pos = {text_char_x(app, text, range[0]),
        sfText_getPosition(text).y};

    if (!part)
        return;
    memcpy(part, str + range[0], sizeof(sfUint32) * (range[1] - range[0]));
    part[range[1] - range[0]] = 0;
    sfText_setFont(run, font);
    sfText_setCharacterSize(run, sfText_getCharacterSize(text));
    sfText_setFillColor(run, sfText_getFillColor(text));
    sfText_setUnicodeString(run, part);
    sfText_setPosition(run, pos);
    sfRenderWindow_drawText(app->window, run, NULL);
    free(part);
}

static void draw_mixed(app_t *app, sfText const *text)
{
    sfUint32 const *str = sfText_getUnicodeString(text);
    unsigned int size = sfText_getCharacterSize(text);
    size_t range[2] = {0, 0};
    sfFont *font = NULL;

    if (!app->run_text || !text_mixed(app, text)) {
        sfRenderWindow_drawText(app->window, text, NULL);
        return;
    }
    while (str[range[0]] != 0) {
        font = text_font(app, str[range[0]], size);
        range[1] = range[0] + 1;
        while (str[range[1]] != 0
            && text_font(app, str[range[1]], size) == font)
            range[1]++;
        draw_run(app, text, range, font);
        range[0] = range[1];
    }
}

/*
** replaces sfRenderWindow_drawText for every text of the program; the
** color is softened in dark mode, then given back to the text
*/
void text_draw(app_t *app, sfText *text)
{
    sfColor color = sfText_getFillColor(text);

    sfText_setFillColor(text, dark_soften(app, color));
    draw_mixed(app, text);
    sfText_setFillColor(text, color);
}
