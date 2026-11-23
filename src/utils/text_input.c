/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** typing in a text buffer (utf-8) of NAME_LEN bytes, or of a given
** size: characters, backspace and Ctrl+V (paste), shared by every
** screen with a field
*/

#include "mp3player.h"

/*
** removes the last character (which may take several bytes in utf-8)
*/
static void erase_last(char *buf)
{
    int n = strlen(buf);

    if (n == 0)
        return;
    n--;
    while (n > 0 && ((unsigned char)buf[n] & 0xC0) == 0x80)
        n--;
    buf[n] = '\0';
}

static void append_code(char *buf, size_t size, sfUint32 code)
{
    char utf8[5] = {0};
    size_t len = utf8_encode(utf8, code);

    if (strlen(buf) + len < size)
        strcat(buf, utf8);
}

/*
** the pasted text stops at the first line break or tab, and is cut
** before a character that would not fit
*/
static void paste(char *buf, size_t size)
{
    sfUint32 const *text = sfClipboard_getUnicodeString();

    for (int i = 0; text && text[i] >= 32 && text[i] != 127; i++)
        append_code(buf, size, text[i]);
}

/*
** 8 = backspace, 22 = Ctrl+V, other codes under 32 are control keys
** (ignored); returns 1 if the buffer (of _size_ bytes) changed
*/
int text_input_n(char *buf, size_t size, sfUint32 code)
{
    if (code == 8) {
        erase_last(buf);
        return 1;
    }
    if (code == 22) {
        paste(buf, size);
        return 1;
    }
    if (code < 32 || code == 127)
        return 0;
    append_code(buf, size, code);
    return 1;
}

int text_input(char *buf, sfUint32 code)
{
    return text_input_n(buf, NAME_LEN, code);
}
