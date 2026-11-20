/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** the library: every song ever seen + the array of the present ones
*/

#include "mp3player.h"
#include <strings.h>

static int compare_songs(void const *a, void const *b)
{
    song_t const *first = *(song_t *const *)a;
    song_t const *second = *(song_t *const *)b;

    return strcasecmp(song_title(first), song_title(second));
}

static int build_present(library_t *lib)
{
    int i = 0;

    lib->nb = 0;
    for (song_t *song = lib->songs; song; song = song->next)
        lib->nb += song->present;
    free(lib->present);
    lib->present = malloc(sizeof(song_t *) * (lib->nb + 1));
    if (!lib->present)
        return 84;
    for (song_t *song = lib->songs; song; song = song->next)
        if (song->present) {
            lib->present[i] = song;
            i++;
        }
    lib->present[i] = NULL;
    qsort(lib->present, lib->nb, sizeof(song_t *), &compare_songs);
    return 0;
}

/*
** the file goes to the song its fingerprint belongs to, or to the song
** that one was merged into; the own files of a song are attached first
** (aliases = 0), then those of its aliases (aliases = 1)
*/
static void attach_file(library_t *lib, fprint_t *fp, int aliases)
{
    song_t *owner = song_by_id(lib, fp->song_id);
    song_t *song = song_resolve(lib, owner);

    if (!fp->path || !song || (owner != song) != aliases)
        return;
    if (song->present) {
        fprintf(stderr, "Already in the library, ignored: %s\n", fp->path);
        return;
    }
    song->present = 1;
    song->path = strdup(fp->path);
    if (fp->file) {
        free(song->file);
        song->file = strdup(fp->file);
    }
}

/*
** to call after any change of the songs (scan, edit, merge)
*/
int library_refresh(library_t *lib)
{
    for (song_t *song = lib->songs; song; song = song->next) {
        song->present = 0;
        free(song->path);
        song->path = NULL;
    }
    for (fprint_t *fp = lib->fprints; fp; fp = fp->next)
        attach_file(lib, fp, 0);
    for (fprint_t *fp = lib->fprints; fp; fp = fp->next)
        attach_file(lib, fp, 1);
    for (song_t *song = lib->songs; song; song = song->next)
        song_build_artists(lib, song);
    return build_present(lib);
}

/*
** empty library holding only the general ranking
*/
static library_t *library_new(void)
{
    library_t *lib = calloc(1, sizeof(library_t));

    if (!lib)
        return NULL;
    lib->next_song = 1;
    lib->next_artist = 1;
    lib->next_playlist = 1;
    lib->next_board = 1;
    if (board_general_create(lib) == 84) {
        library_destroy(lib);
        return NULL;
    }
    return lib;
}

library_t *library_load(void)
{
    library_t *lib = library_new();

    if (!lib)
        return NULL;
    db_load_all(lib);
    db_merge(lib, scan_music(MUSIC_DIR));
    if (library_refresh(lib) == 84) {
        library_destroy(lib);
        return NULL;
    }
    db_load_elo(lib);
    tsv_touch(LISTENS_LOG, LISTENS_COLUMNS);
    tsv_touch(BATTLES_LOG, BATTLES_COLUMNS);
    db_save_all(lib);
    return lib;
}
