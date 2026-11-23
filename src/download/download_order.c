/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** order of an imported playlist: yt-dlp lists the videos in the order
** of the playlist of the site (the newest first for the liked songs);
** for a single link going to a single playlist, the local playlist
** takes this order once the queue is over (the songs already there,
** downloaded or added by hand without a known video go after them)
*/

#include "mp3player.h"

void download_order_free(download_t *dl)
{
    for (int i = 0; i < dl->nb_order; i++)
        free(dl->order[i]);
    free(dl->order);
    dl->order = NULL;
    dl->nb_order = 0;
}

/*
** called before the videos listed are sorted: their ids are kept in
** the order of the list
*/
void download_keep_order(app_t *app)
{
    download_t *dl = &app->download;
    int nb = entries_count(dl->listed);

    download_order_free(dl);
    if (dl->total != 1 || dl->opt.nb_targets != 1 || nb < 2)
        return;
    dl->order = malloc(sizeof(char *) * nb);
    if (!dl->order)
        return;
    for (dl_entry_t *entry = dl->listed; entry; entry = entry->next) {
        dl->order[dl->nb_order] = strdup(entry->id);
        if (dl->order[dl->nb_order])
            dl->nb_order++;
    }
}

/*
** a video no longer served was found again in the library under another
** video (download_reupload.c): the song takes its place in the order
*/
void download_order_swap(download_t *dl, char const *old_id,
    library_t *lib, song_t *song)
{
    char id[VIDEO_ID];

    if (!song_video_id(lib, song, id))
        return;
    for (int i = 0; i < dl->nb_order; i++) {
        if (strcmp(dl->order[i], old_id) != 0)
            continue;
        free(dl->order[i]);
        dl->order[i] = strdup(id);
        if (!dl->order[i])
            dl->order[i] = strdup("");
        return;
    }
}

/*
** place of the song of this raw id in the list (nb_order if it is not
** in it)
*/
static int place(library_t *lib, download_t const *dl, int song_id)
{
    song_t *song = song_resolve(lib, song_by_id(lib, song_id));
    char id[VIDEO_ID];

    if (!song || !song_video_id(lib, song, id))
        return dl->nb_order;
    for (int i = 0; i < dl->nb_order; i++)
        if (strcmp(dl->order[i], id) == 0)
            return i;
    return dl->nb_order;
}

/*
** insertion sort of the songs on their place (stable: the songs out of
** the list keep their order)
*/
static void sort_places(int *songs, int *places, int nb)
{
    int song = 0;
    int at = 0;
    int j = 0;

    for (int i = 1; i < nb; i++) {
        song = songs[i];
        at = places[i];
        j = i;
        while (j > 0 && places[j - 1] > at) {
            songs[j] = songs[j - 1];
            places[j] = places[j - 1];
            j--;
        }
        songs[j] = song;
        places[j] = at;
    }
}

/*
** called when the queue is over (not when it is cancelled)
*/
void download_apply_order(app_t *app)
{
    download_t *dl = &app->download;
    playlist_t *pl = playlist_by_id(app->lib, dl->opt.targets[0]);
    int *places = NULL;

    if (dl->nb_order > 0 && pl && pl->nb > 1)
        places = malloc(sizeof(int) * pl->nb);
    if (places) {
        for (int i = 0; i < pl->nb; i++)
            places[i] = place(app->lib, dl, pl->songs[i]);
        sort_places(pl->songs, places, pl->nb);
        free(places);
        db_save_playlists(app->lib);
    }
    download_order_free(dl);
}
