/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** artists of a song: created from the ID3 tag, displayed as a string
*/

#include "mp3player.h"

/*
** the first two separate main artists, the others introduce featurings
*/
static char const *const separators[] = {";", "/", " feat. ", " ft. ",
    " featuring ", " Feat. ", " Ft. ", " Featuring ", NULL};

static char const *next_separator(char const *str, int *idx)
{
    char const *best = NULL;
    char const *found = NULL;

    for (int i = 0; separators[i]; i++) {
        found = strstr(str, separators[i]);
        if (found && (!best || found < best)) {
            best = found;
            *idx = i;
        }
    }
    return best;
}

static void add_token(library_t *lib, song_t *song, char *token,
    role_t role)
{
    char *start = token;
    int len = 0;
    artist_t *artist = NULL;

    while (*start == ' ')
        start++;
    len = strlen(start);
    while (len > 0 && start[len - 1] == ' ') {
        start[len - 1] = '\0';
        len--;
    }
    if (len > 0)
        artist = artist_get(lib, start);
    if (artist)
        link_add(lib, song->id, artist->id, role);
    free(token);
}

void artists_from_source(library_t *lib, song_t *song)
{
    char const *str = song->src_artists;
    char const *sep = NULL;
    int idx = 0;
    role_t role = ROLE_MAIN;

    while (str && *str) {
        sep = next_separator(str, &idx);
        if (!sep) {
            add_token(lib, song, strdup(str), role);
            return;
        }
        add_token(lib, song, strndup(str, sep - str), role);
        if (idx >= 2)
            role = ROLE_FEAT;
        str = sep + strlen(separators[idx]);
    }
}

static void append_names(library_t *lib, song_t *song, char *buf)
{
    artist_t *artist = NULL;

    for (link_t *link = lib->links; link; link = link->next) {
        if (link->song_id != song->id)
            continue;
        artist = artist_by_id(lib, link->artist_id);
        if (!artist)
            continue;
        if (buf[0] != '\0')
            strcat(buf, ", ");
        strcat(buf, artist->name);
    }
}

/*
** "A, B, C" in the order of the links, like YouTube Music: the roles
** (feat., remix) are kept in the links and shown on the song's page,
** the title says who did what
*/
void song_build_artists(library_t *lib, song_t *song)
{
    size_t size = 16;
    artist_t *artist = NULL;

    for (link_t *link = lib->links; link; link = link->next) {
        if (link->song_id != song->id)
            continue;
        artist = artist_by_id(lib, link->artist_id);
        if (artist)
            size += strlen(artist->name) + 10;
    }
    free(song->artists);
    song->artists = calloc(size, sizeof(char));
    if (!song->artists)
        return;
    append_names(lib, song, song->artists);
    if (song->artists[0] == '\0') {
        free(song->artists);
        song->artists = NULL;
    }
}
