/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** artist names written as a list by YouTube Music ("C418, Baiyon",
** "4444 OF A KIND, D-Block & S-te-Fan, Sub Zero Project"): split into
** one artist per part, each with the role of the whole name. Done on
** its own (library_refresh), except for the doubtful names, which can
** be one artist ("Tyler, The Creator", "Earth, Wind & Fire"): they wait
** in the list "À vérifier" of the download screen (split or keep).
** "&" never splits a name: "D-Block & S-te-Fan" is one artist.
*/

#include "mp3player.h"
#include <strings.h>

/*
** parts : SPLIT_MAX names of NAME_LEN; returns their number (empty
** parts left out)
*/
int artist_split_parts(char const *name, char (*parts)[NAME_LEN])
{
    int nb = 0;
    int len = 0;

    while (*name && nb < SPLIT_MAX) {
        len = strcspn(name, ",");
        snprintf(parts[nb], NAME_LEN, "%.*s", len, name);
        str_trim(parts[nb]);
        if (parts[nb][0] != '\0')
            nb++;
        name += len;
        if (*name == ',')
            name++;
    }
    return nb;
}

/*
** "The X" after a comma, or a "&" in the last part: maybe one artist
*/
int artist_split_doubtful(char (*parts)[NAME_LEN], int nb)
{
    char const *space = NULL;

    for (int i = 1; i < nb; i++) {
        space = strchr(parts[i], ' ');
        if (strncasecmp(parts[i], "the ", 4) == 0
            && !strchr(space + 1, ' '))
            return 1;
    }
    return nb > 1 && strstr(parts[nb - 1], " & ") != NULL;
}

static int has_links(library_t *lib, int artist_id)
{
    for (link_t *link = lib->links; link; link = link->next)
        if (link->artist_id == artist_id)
            return 1;
    return 0;
}

/*
** each song of the artist is linked to every part instead, same role
** (added from the last part: a new link goes in front of the others,
** so the parts keep their order)
*/
void artist_split_apply(library_t *lib, artist_t *artist)
{
    char parts[SPLIT_MAX][NAME_LEN];
    int nb = artist_split_parts(artist->name, parts);
    artist_t *part = NULL;
    link_t **cur = &lib->links;
    link_t *old = NULL;

    for (link_t *link = lib->links; link; link = link->next)
        for (int i = nb - 1; link->artist_id == artist->id && i >= 0; i--) {
            part = artist_get(lib, parts[i]);
            if (part && part != artist)
                link_add(lib, link->song_id, part->id, link->role);
        }
    while (*cur) {
        if ((*cur)->artist_id != artist->id) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old);
    }
}

/*
** 1 if every part is already an artist on its own ("The Chainsmokers"
** known elsewhere: the name is surely a list)
*/
static int all_known(library_t *lib, char (*parts)[NAME_LEN], int nb)
{
    for (int i = 0; i < nb; i++)
        if (!artist_find(lib, parts[i]))
            return 0;
    return 1;
}

/*
** 0: nothing to do, 1: split on its own, 2: doubtful, waits for a choice
*/
int artist_split_state(library_t *lib, artist_t const *artist)
{
    char parts[SPLIT_MAX][NAME_LEN];
    split_choice_t const *choice = NULL;
    int nb = 0;

    if (!strchr(artist->name, ',')
        || artist_split_parts(artist->name, parts) < 2
        || !has_links(lib, artist->id))
        return 0;
    nb = artist_split_parts(artist->name, parts);
    choice = split_choice_find(lib, artist->name);
    if (choice)
        return choice->keep ? 0 : 1;
    if (!artist_split_doubtful(parts, nb) || all_known(lib, parts, nb))
        return 1;
    return 2;
}

/*
** in library_refresh, before the artists of the songs are written
*/
void artists_split_auto(library_t *lib)
{
    for (artist_t *artist = lib->artists; artist; artist = artist->next)
        if (artist_split_state(lib, artist) == 1)
            artist_split_apply(lib, artist);
}
