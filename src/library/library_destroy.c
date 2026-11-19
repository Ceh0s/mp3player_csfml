/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** frees the library
*/

#include "mp3player.h"

static void destroy_fprints(fprint_t *fp)
{
    fprint_t *next = NULL;

    while (fp) {
        next = fp->next;
        free(fp->hash);
        free(fp->path);
        free(fp->file);
        free(fp);
        fp = next;
    }
}

static void destroy_artists(artist_t *artist)
{
    artist_t *next = NULL;

    while (artist) {
        next = artist->next;
        free(artist->name);
        free(artist);
        artist = next;
    }
}

static void destroy_links(link_t *link)
{
    link_t *next = NULL;

    while (link) {
        next = link->next;
        free(link);
        link = next;
    }
}

void library_destroy(library_t *lib)
{
    if (!lib)
        return;
    song_list_destroy(lib->songs);
    destroy_fprints(lib->fprints);
    destroy_artists(lib->artists);
    destroy_links(lib->links);
    elo_clear(lib);
    free(lib->playlist);
    free(lib);
}
