/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** song of each excluded video, needed to forget it ("Oublier"): an
** exclusion removes the file, so the video is no longer known by the
** fingerprints; edits.log keeps the removed file ("file" line: song id,
** path ending with "[<video id>].mp3"). Read once, when the list of
** the excluded videos is shown; ex->song_id = 0 if no song was found.
*/

#include "mp3player.h"

/*
** the excluded video whose id is written as "[id]" in path, if any
*/
static rejected_t *video_of(library_t *lib, char const *path)
{
    char mark[VIDEO_ID + 4];

    for (rejected_t *ex = lib->excluded; ex; ex = ex->next) {
        if (ex->song_id >= 0)
            continue;
        snprintf(mark, sizeof(mark), "[%s]", ex->id);
        if (strstr(path, mark))
            return ex;
    }
    return NULL;
}

/*
** edits.log: timestamp, song id, field, before, after
*/
static void read_edits(library_t *lib, FILE *file)
{
    char *line = NULL;
    size_t size = 0;
    char *fields[6];
    rejected_t *ex = NULL;

    while (getline(&line, &size, file) > 0) {
        if (line[0] == '#' || split_tab(line, fields, 6) < 4
            || strcmp(fields[2], "file") != 0)
            continue;
        ex = video_of(lib, fields[3]);
        if (ex)
            ex->song_id = atoi(fields[1]);
    }
    free(line);
}

/*
** a video still in music/ is found by its fingerprint first
*/
void excluded_songs(library_t *lib)
{
    FILE *file = NULL;
    song_t *song = NULL;

    for (rejected_t *ex = lib->excluded; ex; ex = ex->next) {
        song = ex->song_id < 0 ? song_by_video(lib, ex->id) : NULL;
        if (song)
            ex->song_id = song->id;
    }
    file = fopen(EDITS_LOG, "r");
    if (file) {
        read_edits(lib, file);
        fclose(file);
    }
    for (rejected_t *ex = lib->excluded; ex; ex = ex->next)
        if (ex->song_id < 0)
            ex->song_id = 0;
}
