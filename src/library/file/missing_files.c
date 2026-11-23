/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** after files were removed from music/ by the app: only the files that
** are gone are forgotten, instead of reading the whole folder again
** (library_rescan reads the fingerprint of every mp3, half a second)
*/

#include "mp3player.h"

static int gone(char const *path)
{
    return path && access(path, F_OK) != 0;
}

static void forget_fprint(fprint_t *fp)
{
    if (!gone(fp->path))
        return;
    free(fp->path);
    free(fp->file);
    free(fp->source);
    fp->path = NULL;
    fp->file = NULL;
    fp->source = NULL;
}

static void drop_same_files(library_t *lib)
{
    same_file_t **at = &lib->same_files;
    same_file_t *same = NULL;

    while (*at) {
        same = *at;
        if (!gone(same->path)) {
            at = &same->next;
            continue;
        }
        *at = same->next;
        free(same->path);
        free(same->file);
        free(same->source);
        free(same);
    }
}

/*
** the songs whose files are gone become absent (stats kept)
*/
void library_drop_missing(app_t *app)
{
    for (fprint_t *fp = app->lib->fprints; fp; fp = fp->next)
        forget_fprint(fp);
    drop_same_files(app->lib);
    library_changed(app);
}
