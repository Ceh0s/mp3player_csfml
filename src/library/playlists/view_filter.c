/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** filter of a list of songs by a text (search field): the songs whose
** title or artists hold it, without case, stay in their order
*/

#include "mp3player.h"

/*
** 1 if the title or the artists hold text (always if text is empty)
*/
int song_matches(song_t const *song, char const *text)
{
    if (!text || text[0] == '\0')
        return 1;
    return str_contains(song_title(song), text)
        || (song->artists && str_contains(song->artists, text));
}

/*
** keeps the matching songs at the start of view; returns their number
*/
int view_filter(song_t **view, int nb, char const *text)
{
    int kept = 0;

    for (int i = 0; i < nb; i++)
        if (song_matches(view[i], text)) {
            view[kept] = view[i];
            kept++;
        }
    view[kept] = NULL;
    return kept;
}
