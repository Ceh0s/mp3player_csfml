/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** splits a string into an array of words (port of stwa_delimiter from
** the author's wlib, with the libc instead of the my_* functions).
** _didx_ : 0 to cut on any character of _delim_, 1 to cut on the
** _delim_ string itself. _empty_ : 1 if two consecutive delimiters give
** an empty string in the array.
*/

#include "mp3player.h"

static int is_delim(char const *str, char const *delim, int didx)
{
    if (didx == 0)
        return *str != '\0' && strchr(delim, *str) != NULL;
    return strncmp(str, delim, strlen(delim)) == 0;
}

static size_t count_words(char const *str, char const *delim,
    int didx, int empty)
{
    size_t count = (str && empty == 1) ? 1 : 0;
    size_t lend = (didx == 1) ? strlen(delim) : 1;
    int in_word = 0;

    if (!str)
        return 0;
    for (size_t i = 0; str[i];)
        if (is_delim(str + i, delim, didx)) {
            count += empty;
            in_word = 0;
            i += lend;
        } else {
            count += (in_word == 0 && empty == 0) ? 1 : 0;
            in_word = 1;
            i++;
        }
    return count;
}

static char *find_words(char const *str, size_t loc, char const *delim,
    int const *opt)
{
    size_t i = 0;
    size_t count = (opt[1] == 1) ? 1 : 0;
    size_t lend = (opt[0] == 1) ? strlen(delim) : 1;
    size_t len = 0;

    for (int in_word = 0; str[i] && count < loc;)
        if (is_delim(str + i, delim, opt[0])) {
            count += opt[1];
            in_word = 0;
            i += lend;
        } else {
            count += (in_word == 0 && opt[1] == 0) ? 1 : 0;
            in_word = 1;
            i += (count < loc) ? 1 : 0;
        }
    while (str[i + len] && !is_delim(str + i + len, delim, opt[0]))
        len++;
    return strndup(str + i, len);
}

char **stwa_delimiter(char const *str, char const *delim, int didx,
    int empty)
{
    size_t i = 0;
    size_t words = count_words(str, delim, didx, empty);
    char **arr = malloc(sizeof(char *) * (words + 1));

    if (!arr)
        return NULL;
    for (; i < words; i++)
        arr[i] = find_words(str, i + 1, delim, (int [2]){didx, empty});
    arr[i] = NULL;
    return arr;
}

void str_array_free(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}
