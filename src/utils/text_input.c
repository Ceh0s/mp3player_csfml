/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** typing in a text buffer (utf-8) of NAME_LEN bytes, or of a given
** size, at the cursor (text_caret.c): characters, backspace and Ctrl+V
** (paste), shared by every screen with a field
*/

#include "mp3player.h"

/*
** the pasted text stops at the first line break or tab, and is cut
** before a character that would not fit
*/
static void paste(app_t *app, char *buf, size_t size)
{
    sfUint32 const *text = sfClipboard_getUnicodeString();

    for (int i = 0; text && text[i] >= 32 && text[i] != 127; i++)
        caret_insert(app, buf, size, text[i]);
}

/*
** 8 = backspace, 22 = Ctrl+V, other codes under 32 are control keys
** (ignored, Delete is a key of its own: caret_key); returns 1 if the
** buffer (of _size_ bytes) changed
*/
int text_input_n(app_t *app, char *buf, size_t size, sfUint32 code)
{
    if (code == 8)
        return caret_erase(app, buf, 0);
    if (code == 22) {
        paste(app, buf, size);
        return 1;
    }
    if (code < 32 || code == 127)
        return 0;
    return caret_insert(app, buf, size, code);
}

int text_input(app_t *app, char *buf, sfUint32 code)
{
    return text_input_n(app, buf, NAME_LEN, code);
}
