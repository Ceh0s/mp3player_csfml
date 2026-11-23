/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** removal of the files of a song from music/, only when asked (songs
** absent from a playlist, duplicates): the song keeps its stats in
** data/ and only becomes absent (present = 0), like a file removed by
** hand. Each removed file is written in edits.log.
*/

#include "mp3player.h"

void song_log_delete(int song_id, char const *path)
{
    char line[PATH_LEN + 64];

    snprintf(line, sizeof(line), "%d\tfile\t%s\t(supprimé)", song_id,
        path);
    log_line(EDITS_LOG, EDITS_COLUMNS, line);
}

/*
** every file of the song, also those of the songs merged into it, with
** no new scan (song_delete_files, or once after several songs)
*/
void song_remove_files(library_t *lib, song_t *song)
{
    for (fprint_t *fp = lib->fprints; fp; fp = fp->next) {
        if (!fp->path || song_resolve(lib, song_by_id(lib, fp->song_id))
            != song)
            continue;
        song_log_delete(fp->song_id, fp->path);
        if (unlink(fp->path) != 0)
            fprintf(stderr, "Cannot remove %s\n", fp->path);
    }
    same_files_of(lib, song);
}

/*
** the files gone are forgotten: the song becomes absent
*/
void song_delete_files(app_t *app, song_t *song)
{
    song_remove_files(app->lib, song);
    library_drop_missing(app);
    if (app->libscene)
        list_create(app);
}
