/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** songs absent from a playlist of a site: when one address is listed
** for one local playlist (a synchronisation), the songs of the local
** playlist downloaded from a video no longer listed are reported. The
** songs without a known video (added by hand) are never reported.
*/

#include "mp3player.h"

/*
** 1 if one of the files of the song comes from a video, and none of
** them is listed
*/
static int song_absent(library_t *lib, song_t *song, dl_entry_t *listed)
{
    char id[VIDEO_ID];
    int has_id = 0;

    for (fprint_t *fp = lib->fprints; fp; fp = fp->next) {
        if (song_resolve(lib, song_by_id(lib, fp->song_id)) != song
            || !fprint_video_id(fp, id))
            continue;
        if (entry_find(listed, id))
            return 0;
        has_id = 1;
    }
    return has_id;
}

static int already(download_t const *dl, int id)
{
    for (int i = 0; i < dl->nb_absent; i++)
        if (dl->absent[i] == id)
            return 1;
    return 0;
}

static void add_absents(app_t *app, download_t *dl, playlist_t *pl)
{
    song_t *song = NULL;

    for (int i = 0; i < pl->nb; i++) {
        song = song_resolve(app->lib, song_by_id(app->lib, pl->songs[i]));
        if (song && !already(dl, song->id)
            && song_absent(app->lib, song, dl->listed)) {
            dl->absent[dl->nb_absent] = song->id;
            dl->nb_absent++;
        }
    }
}

/*
** called before the videos listed are sorted (only after a listing that
** succeeded and found videos: an empty list from an error would report
** every song); replaces the songs reported before
*/
void download_find_absent(app_t *app)
{
    download_t *dl = &app->download;
    playlist_t *pl = playlist_by_id(app->lib, dl->opt.targets[0]);

    if (!dl->opt.absent || dl->total != 1 || dl->opt.nb_targets != 1
        || !pl || !dl->listed)
        return;
    free(dl->absent);
    dl->nb_absent = 0;
    dl->absent_pl = pl->id;
    dl->absent = malloc(sizeof(int) * (pl->nb + 1));
    if (dl->absent)
        add_absents(app, dl, pl);
}

/*
** the song stays where it is: it is only taken out of the report
*/
void download_absent_keep(app_t *app, int i)
{
    download_t *dl = &app->download;

    if (i < 0 || i >= dl->nb_absent)
        return;
    dl->nb_absent--;
    memmove(&dl->absent[i], &dl->absent[i + 1],
        sizeof(int) * (dl->nb_absent - i));
}

void download_absent_unlist(app_t *app, int i)
{
    download_t *dl = &app->download;
    playlist_t *pl = playlist_by_id(app->lib, dl->absent_pl);
    song_t *song = NULL;

    if (i < 0 || i >= dl->nb_absent)
        return;
    song = song_by_id(app->lib, dl->absent[i]);
    if (pl && song && playlist_has_song(app->lib, pl, song)) {
        playlist_toggle(app->lib, pl, song);
        db_save_playlists(app->lib);
    }
    download_absent_keep(app, i);
}

/*
** taken out of the playlist, then its files are removed from music/
*/
void download_absent_delete(app_t *app, int i)
{
    download_t *dl = &app->download;
    song_t *song = NULL;

    if (i < 0 || i >= dl->nb_absent)
        return;
    song = song_by_id(app->lib, dl->absent[i]);
    download_absent_unlist(app, i);
    if (song)
        song_delete_files(app, song);
}
