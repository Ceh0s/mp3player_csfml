/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** typing in a text buffer of NAME_LEN bytes (utf-8): characters,
** backspace and Ctrl+V (paste), shared by every screen with a field
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

static void append_code(char *buf, sfUint32 code)
{
    char utf8[5] = {0};
    int len = utf8_encode(utf8, code);

    if (strlen(buf) + len < NAME_LEN)
        strcat(buf, utf8);
}

/*
** the pasted text stops at the first line break or tab, and is cut
** before a character that would not fit
*/
static void paste(char *buf)
{
    sfUint32 const *text = sfClipboard_getUnicodeString();

    for (int i = 0; text && text[i] >= 32 && text[i] != 127; i++)
        append_code(buf, text[i]);
}

/*
** 8 = backspace, 22 = Ctrl+V, other codes under 32 are control keys
** (ignored); returns 1 if the buffer changed
*/
int text_input(char *buf, sfUint32 code)
{
    if (code == 8) {
        erase_last(buf);
        return 1;
    }
    if (code == 22) {
        paste(buf);
        return 1;
    }
    if (code < 32 || code == 127)
        return 0;
    append_code(buf, code);
    return 1;
}
