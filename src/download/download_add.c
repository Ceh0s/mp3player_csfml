/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a video was just downloaded: its mp3 (named "title [id].mp3") is
** added to the library alone, without scanning the whole folder again,
** then to the target playlists of the download
*/

#include "mp3player.h"
#include <dirent.h>

/*
** name of the file of the video in music/, or NULL
*/
static char *find_file(char const *id)
{
    DIR *folder = opendir(MUSIC_DIR);
    struct dirent *entry = NULL;
    char *name = NULL;

    if (!folder)
        return NULL;
    for (entry = readdir(folder); entry && !name; entry = readdir(folder))
        if (video_in_name(entry->d_name, id))
            name = strdup(entry->d_name);
    closedir(folder);
    return name;
}

/*
** the whole folder is scanned again if the file was not found (a site
** naming its files differently)
*/
static void add_file(app_t *app, char const *id)
{
    char *name = find_file(id);

    if (!name) {
        library_rescan(app);
        return;
    }
    db_merge(app->lib, scan_file(MUSIC_DIR, name));
    free(name);
    library_changed(app);
    images_from_covers(app->lib);
}

void download_added(app_t *app, dl_entry_t const *entry)
{
    song_t *song = NULL;

    if (!entry)
        return;
    add_file(app, entry->id);
    song = song_by_video(app->lib, entry->id);
    if (song && playlists_append(app->lib, entry->targets,
            entry->nb_targets, song) > 0)
        db_save_playlists(app->lib);
    if (app->libscene)
        list_refresh(app);
}
