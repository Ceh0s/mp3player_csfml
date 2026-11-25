/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** frees the library
*/

#include "mp3player.h"

/*
** the fingerprints, and the files with the sound of one of them
*/
static void destroy_fprints(library_t *lib)
{
    fprint_t *fp = lib->fprints;
    fprint_t *next = NULL;

    while (fp) {
        next = fp->next;
        free(fp->hash);
        free(fp->path);
        free(fp->file);
        free(fp->source);
        free(fp);
        fp = next;
    }
    same_files_free(lib);
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

/*
** the lists about the videos of the sites
*/
static void destroy_videos(library_t *lib)
{
    rejected_remove(lib, NULL);
    excluded_remove(lib, NULL);
    unavailable_remove(lib, NULL);
    split_choices_free(lib);
    source_remove(lib, -1);
}

void library_destroy(library_t *lib)
{
    if (!lib)
        return;
    song_list_destroy(lib->songs);
    destroy_fprints(lib);
    destroy_artists(lib->artists);
    destroy_links(lib->links);
    elo_clear(lib);
    playlist_remove(lib, NULL);
    sort_remove(lib, -1);
    board_remove(lib, NULL);
    destroy_videos(lib);
    envelopes_free(lib);
    not_dups_free(lib);
    ties_free(lib);
    likes_free(lib);
    suggest_states_free(lib);
    notes_remove(lib, 0);
    periods_forget(lib, 0, 0);
    video_aliases_free(lib);
    free(lib->present);
    free(lib);
}
