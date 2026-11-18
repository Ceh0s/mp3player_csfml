/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** links the files found in the music folder to the saved songs,
** using their audio fingerprint: nothing is ever deleted
*/

#include "mp3player.h"

static void replace(char **dst, char **src)
{
    if (!*src)
        return;
    free(*dst);
    *dst = *src;
    *src = NULL;
}

/*
** the tags of the file are refreshed, but never the title and the
** artists, which may have been edited in the app
*/
static void update_entry(song_t *old, song_t *found)
{
    replace(&old->src_title, &found->src_title);
    replace(&old->src_artists, &found->src_artists);
    replace(&old->album, &found->album);
    replace(&old->genre, &found->genre);
    replace(&old->year, &found->year);
    if (found->duration_ms > 0)
        old->duration_ms = found->duration_ms;
    old->last_seen = time(NULL);
    free(old->key);
    old->key = NULL;
}

static song_t *create_entry(library_t *lib, song_t *found)
{
    song_t *last = lib->songs;

    found->id = lib->next_song;
    lib->next_song++;
    found->title = found->src_title ? strdup(found->src_title) : NULL;
    found->first_seen = time(NULL);
    found->last_seen = found->first_seen;
    free(found->key);
    found->key = NULL;
    while (last && last->next)
        last = last->next;
    if (last)
        last->next = found;
    else
        lib->songs = found;
    artists_from_source(lib, found);
    return found;
}

/*
** only songs migrated from the v1 format still have a key
*/
static song_t *find_by_key(library_t *lib, char const *key)
{
    for (song_t *song = lib->songs; key && song; song = song->next)
        if (song->key && strcmp(song->key, key) == 0)
            return song;
    return NULL;
}

/*
** the fingerprint of the file now belongs to song, and knows the file
*/
static void attach_print(library_t *lib, fprint_t *fp, song_t *found,
    song_t *song)
{
    if (!fp)
        fp = fprint_add(lib, found->hash, song->id);
    if (!fp)
        return;
    fp->song_id = song->id;
    replace(&fp->path, &found->path);
    replace(&fp->file, &found->file);
}

static void merge_one(library_t *lib, song_t *found)
{
    fprint_t *fp = fprint_find(lib, found->hash);
    song_t *song = NULL;

    if (fp && fp->path) {
        fprintf(stderr, "Same file twice, ignored: %s\n", found->path);
        song_destroy(found);
        return;
    }
    song = fp ? song_by_id(lib, fp->song_id) : find_by_key(lib, found->key);
    if (song)
        update_entry(song, found);
    else
        song = create_entry(lib, found);
    attach_print(lib, fp, found, song);
    if (song != found)
        song_destroy(found);
}

void db_merge(library_t *lib, song_t *scanned)
{
    song_t *next = NULL;

    while (scanned) {
        next = scanned->next;
        scanned->next = NULL;
        merge_one(lib, scanned);
        scanned = next;
    }
}
