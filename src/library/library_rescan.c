/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scans the music folder again while the app runs (after a download):
** new files become new songs, known ones are found by their fingerprint
*/

#include "mp3player.h"

/*
** the paths are forgotten first, as at launch: otherwise every known
** file would be seen as a second copy of itself
*/
void library_rescan(app_t *app)
{
    for (fprint_t *fp = app->lib->fprints; fp; fp = fp->next) {
        free(fp->path);
        free(fp->file);
        free(fp->source);
        fp->path = NULL;
        fp->file = NULL;
        fp->source = NULL;
    }
    same_files_free(app->lib);
    db_merge(app->lib, scan_music(MUSIC_DIR));
    library_changed(app);
    images_from_covers(app->lib);
}
