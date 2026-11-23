/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** moving in a utf-8 text one character at a time (a character takes 1
** to 4 bytes, the following bytes start with the bits 10)
*/

#include "mp3player.h"

static int following(char c)
{
    return ((unsigned char)c & 0xC0) == 0x80;
}

/*
** the start of the character before the byte pos
*/
size_t utf8_prev(char const *str, size_t pos)
{
    if (pos == 0)
        return 0;
    pos--;
    while (pos > 0 && following(str[pos]))
        pos--;
    return pos;
}

/*
** the start of the character after the one at the byte pos
*/
size_t utf8_next(char const *str, size_t pos)
{
    if (str[pos] == '\0')
        return pos;
    pos++;
    while (str[pos] != '\0' && following(str[pos]))
        pos++;
    return pos;
}

/*
** number of characters in the first bytes of str
*/
int utf8_count(char const *str, size_t bytes)
{
    int nb = 0;

    for (size_t i = 0; i < bytes && str[i] != '\0'; i++)
        nb += !following(str[i]);
    return nb;
}
