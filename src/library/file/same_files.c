/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** files whose sound is exactly the one of another file (two videos of
** the same recording, same fingerprint): they are not songs of their
** own, but their video is known (never downloaded again) and the
** duplicates screen proposes to remove them. Found again at each scan.
*/

#include "mp3player.h"

/*
** found gives its path, name and address to the list
*/
void same_file_add(library_t *lib, song_t *found, int song_id)
{
    same_file_t *same = calloc(1, sizeof(same_file_t));
    same_file_t **last = &lib->same_files;

    if (!same)
        return;
    same->path = found->path;
    same->file = found->file;
    same->source = found->source;
    same->song_id = song_id;
    found->path = NULL;
    found->file = NULL;
    found->source = NULL;
    while (*last)
        last = &(*last)->next;
    *last = same;
}

void same_files_free(library_t *lib)
{
    same_file_t *next = NULL;

    while (lib->same_files) {
        next = lib->same_files->next;
        free(lib->same_files->path);
        free(lib->same_files->file);
        free(lib->same_files->source);
        free(lib->same_files);
        lib->same_files = next;
    }
}

/*
** the song holding the sound of the video of this id, or NULL
*/
song_t *same_file_video(library_t *lib, char const *id)
{
    char found[12];
    int known = 0;

    for (same_file_t *same = lib->same_files; same; same = same->next) {
        known = source_video_id(same->source, found)
            && strcmp(found, id) == 0;
        if (known || video_in_name(same->path, id))
            return song_resolve(lib, song_by_id(lib, same->song_id));
    }
    return NULL;
}

/*
** removes the file from music/ (written in edits.log); the list is
** rebuilt by the next scan
*/
void same_file_delete(same_file_t *same)
{
    song_log_delete(same->song_id, same->path);
    if (unlink(same->path) != 0)
        fprintf(stderr, "Cannot remove %s\n", same->path);
}

/*
** the files with the sound of the song are removed with it
*/
void same_files_of(library_t *lib, song_t *song)
{
    for (same_file_t *same = lib->same_files; same; same = same->next)
        if (song_resolve(lib, song_by_id(lib, same->song_id)) == song)
            same_file_delete(same);
}
