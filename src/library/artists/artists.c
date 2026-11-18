/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** artists and the links between songs and artists
*/

#include "mp3player.h"
#include <strings.h>

artist_t *artist_find(library_t *lib, char const *name)
{
    for (artist_t *artist = lib->artists; artist; artist = artist->next)
        if (strcasecmp(artist->name, name) == 0)
            return artist;
    return NULL;
}

artist_t *artist_get(library_t *lib, char const *name)
{
    artist_t *artist = artist_find(lib, name);

    if (artist)
        return artist;
    artist = calloc(1, sizeof(artist_t));
    if (!artist)
        return NULL;
    artist->name = strdup(name);
    if (!artist->name) {
        free(artist);
        return NULL;
    }
    artist->id = lib->next_artist;
    lib->next_artist++;
    artist->next = lib->artists;
    lib->artists = artist;
    return artist;
}

artist_t *artist_by_id(library_t *lib, int id)
{
    for (artist_t *artist = lib->artists; artist; artist = artist->next)
        if (artist->id == id)
            return artist;
    return NULL;
}

int link_add(library_t *lib, int song_id, int artist_id, role_t role)
{
    link_t *link = lib->links;

    for (; link; link = link->next)
        if (link->song_id == song_id && link->artist_id == artist_id)
            return 0;
    link = calloc(1, sizeof(link_t));
    if (!link)
        return 84;
    link->song_id = song_id;
    link->artist_id = artist_id;
    link->role = role;
    link->next = lib->links;
    lib->links = link;
    return 0;
}

void links_remove_song(library_t *lib, int song_id)
{
    link_t **link = &lib->links;
    link_t *old = NULL;

    while (*link) {
        if ((*link)->song_id != song_id) {
            link = &(*link)->next;
            continue;
        }
        old = *link;
        *link = old->next;
        free(old);
    }
}
