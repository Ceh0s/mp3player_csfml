/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** small string helpers
*/

#include "mp3player.h"

char *str_lower(char const *str)
{
    char *low = strdup(str);

    if (!low)
        return NULL;
    for (int i = 0; low[i]; i++)
        if (low[i] >= 'A' && low[i] <= 'Z')
            low[i] += 'a' - 'A';
    return low;
}

char *str_or_null(char const *str)
{
    if (!str || str[0] == '\0')
        return NULL;
    return strdup(str);
}

int split_tab(char *line, char **fields, int max)
{
    int n = 0;
    int len = strlen(line);

    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        line[len - 1] = '\0';
        len--;
    }
    fields[n] = line;
    n++;
    for (int i = 0; line[i] && n < max; i++) {
        if (line[i] == '\t') {
            line[i] = '\0';
            fields[n] = &line[i + 1];
            n++;
        }
    }
    return n;
}

/*
** rounded to the nearest second, so that parts add up to their total
*/
void format_time(char *buf, size_t size, long ms)
{
    long sec = (ms + 500) / 1000;

    if (sec >= 3600)
        snprintf(buf, size, "%ld:%02ld:%02ld", sec / 3600,
            (sec / 60) % 60, sec % 60);
    else
        snprintf(buf, size, "%ld:%02ld", sec / 60, sec % 60);
}

/*
** case insensitive search of needle in hay (an empty needle matches)
*/
int str_contains(char const *hay, char const *needle)
{
    char *low_hay = str_lower(hay);
    char *low_needle = str_lower(needle);
    int found = 0;

    if (low_hay && low_needle)
        found = strstr(low_hay, low_needle) != NULL;
    free(low_hay);
    free(low_needle);
    return found;
}

/*
** the spaces at both ends of str are removed (in place)
*/
void str_trim(char *str)
{
    int len = strlen(str);
    int start = 0;

    while (len > 0 && str[len - 1] == ' ')
        len--;
    str[len] = '\0';
    while (str[start] == ' ')
        start++;
    memmove(str, str + start, strlen(str + start) + 1);
}
