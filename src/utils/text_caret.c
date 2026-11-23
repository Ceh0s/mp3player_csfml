/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** cursor of the field receiving the keys (app->wg->caret): typing and
** erasing happen where it is; the arrows, Home and End move it, Delete
** erases the character after it
*/

#include "mp3player.h"

/*
** the cursor of buf: at the end for a field that did not have it, and
** kept inside the text (which may have been changed or emptied since)
*/
caret_t *caret_of(app_t *app, char *buf)
{
    caret_t *caret = &app->wg->caret;
    size_t len = strlen(buf);

    if (caret->buf != buf) {
        caret->buf = buf;
        caret->pos = len;
        caret->first = 0;
    }
    if (caret->pos > len)
        caret->pos = len;
    if (buf[caret->pos] != '\0')
        caret->pos = utf8_prev(buf, utf8_next(buf, caret->pos));
    if (caret->first > caret->pos)
        caret->first = caret->pos;
    return caret;
}

/*
** returns 1 if the character fits in the size bytes of buf
*/
int caret_insert(app_t *app, char *buf, size_t size, sfUint32 code)
{
    caret_t *caret = caret_of(app, buf);
    char utf8[5] = {0};
    size_t len = utf8_encode(utf8, code);
    size_t total = strlen(buf);

    if (total + len >= size)
        return 0;
    memmove(buf + caret->pos + len, buf + caret->pos,
        total - caret->pos + 1);
    memcpy(buf + caret->pos, utf8, len);
    caret->pos += len;
    return 1;
}

/*
** forward = 0 : the character before the cursor (backspace), 1 : the
** one after it (Delete); returns 1 if one was erased
*/
int caret_erase(app_t *app, char *buf, int forward)
{
    caret_t *caret = caret_of(app, buf);
    size_t from = forward ? caret->pos : utf8_prev(buf, caret->pos);
    size_t to = forward ? utf8_next(buf, caret->pos) : caret->pos;

    if (from == to)
        return 0;
    memmove(buf + from, buf + to, strlen(buf + to) + 1);
    caret->pos = from;
    return 1;
}

static int caret_code(sfKeyCode code)
{
    return code == sfKeyLeft || code == sfKeyRight || code == sfKeyHome
        || code == sfKeyEnd || code == sfKeyDelete;
}

/*
** returns 1 if the key was for the field drawn with the focus
*/
int caret_key(app_t *app, sfEvent const *event)
{
    caret_t *caret = &app->wg->caret;
    sfKeyCode code = event->key.code;

    if (event->type != sfEvtKeyPressed || !caret->buf || !caret->shown
        || !caret_code(code))
        return 0;
    caret_of(app, caret->buf);
    if (code == sfKeyLeft)
        caret->pos = utf8_prev(caret->buf, caret->pos);
    if (code == sfKeyRight)
        caret->pos = utf8_next(caret->buf, caret->pos);
    if (code == sfKeyHome)
        caret->pos = 0;
    if (code == sfKeyEnd)
        caret->pos = strlen(caret->buf);
    if (code == sfKeyDelete)
        caret_erase(app, caret->buf, 1);
    return 1;
}
