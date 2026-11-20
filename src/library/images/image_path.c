/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** cleaning of a path pasted in a field: a file manager may give
** "file:///home/me/my%20image.png", with quotes or spaces around it
*/

#include "mp3player.h"
#include <ctype.h>

static int hex_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    return tolower(c) - 'a' + 10;
}

/*
** %20 -> ' ' (only in a file:// address)
*/
static void decode_percent(char *path)
{
    int out = 0;

    for (int i = 0; path[i]; i++) {
        if (path[i] == '%' && isxdigit(path[i + 1])
            && isxdigit(path[i + 2])) {
            path[out] = hex_value(path[i + 1]) * 16 + hex_value(path[i + 2]);
            i += 2;
        } else
            path[out] = path[i];
        out++;
    }
    path[out] = '\0';
}

/*
** out holds NAME_LEN bytes
*/
void path_clean(char const *in, char *out)
{
    int uri = 0;
    int len = 0;

    while (*in == ' ' || *in == '\'' || *in == '"')
        in++;
    uri = strncmp(in, "file://", 7) == 0;
    snprintf(out, NAME_LEN, "%s", uri ? in + 7 : in);
    len = strlen(out);
    while (len > 0 && (out[len - 1] == ' ' || out[len - 1] == '\''
            || out[len - 1] == '"' || out[len - 1] == '\n'))
        len--;
    out[len] = '\0';
    if (uri)
        decode_percent(out);
}
