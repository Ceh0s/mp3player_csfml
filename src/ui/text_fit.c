/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a text too long for its place ends with "…": the most characters
** that fit are kept (never half of one), found by halving the length
** (a few measures per text). The settings can turn it off: the text is
** then written whole.
*/

#include "mp3player.h"

/*
** the start of the character holding the byte pos
*/
static size_t char_start(char const *str, size_t pos)
{
    if (pos == 0)
        return 0;
    return utf8_prev(str, pos + 1);
}

/*
** width of the first len bytes of str followed by "…" (the text keeps
** this string)
*/
static float cut_width(app_t *app, sfText *text, char const *str, size_t len)
{
    char buf[FIT_LEN + 4];

    if (len > FIT_LEN)
        len = FIT_LEN;
    memcpy(buf, str, len);
    strcpy(buf + len, "…");
    text_set(text, buf);
    return text_width(app, text);
}

/*
** the character size of text must be set before; max_w : width allowed
*/
void text_fit(app_t *app, sfText *text, char const *str, float max_w)
{
    size_t keep = 0;
    size_t over = strlen(str);
    size_t mid = 0;

    text_set(text, str);
    if (!app->lib->settings.ellipsis
        || text_width(app, text) <= max_w)
        return;
    while (over - keep > 1) {
        mid = char_start(str, (keep + over) / 2);
        if (mid <= keep)
            mid = utf8_next(str, keep);
        if (mid >= over)
            break;
        if (cut_width(app, text, str, mid) <= max_w)
            keep = mid;
        else
            over = mid;
    }
    cut_width(app, text, str, keep);
}
