/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** actions on every song selected in the library: opinion, playlist,
** title suggestions, image, exclusion and removal of the files; each
** one writes its result in sel->msg and saves once at the end
*/

#include "mp3player.h"

void bulk_like(app_t *app, int value)
{
    selection_t *sel = &app->libscene->sel;

    for (int i = 0; i < sel->nb; i++)
        if (select_song(app, i))
            like_put(app->lib, KIND_SONG, select_song(app, i)->id, value);
    likes_sync(app->lib);
    db_save_likes(app->lib);
    db_save_playlists(app->lib);
    library_reorder(app, 1);
    snprintf(sel->msg, sizeof(sel->msg), "Avis donné à %d titre(s).",
        sel->nb);
}

/*
** add = 1 adds the songs to the playlist chosen, 0 removes them
*/
void bulk_playlist(app_t *app, int add)
{
    selection_t *sel = &app->libscene->sel;
    playlist_t *pl = playlist_by_id(app->lib, sel->playlist);
    song_t *song = NULL;
    int done = 0;

    for (int i = 0; pl && i < sel->nb; i++) {
        song = select_song(app, i);
        if (!song || playlist_has_song(app->lib, pl, song) == add)
            continue;
        done += playlist_toggle(app->lib, pl, song) == 0;
    }
    db_save_playlists(app->lib);
    library_reorder(app, 1);
    snprintf(sel->msg, sizeof(sel->msg), "%d titre(s) %s.", done,
        add ? "ajouté(s)" : "retiré(s)");
}

/*
** from_cover = 1 takes the cover of each mp3, 0 the image of sel->image
*/
void bulk_images(app_t *app, int from_cover)
{
    selection_t *sel = &app->libscene->sel;
    rank_t rank = {NULL, NULL, 0, 0, 0};
    int done = 0;

    for (int i = 0; i < sel->nb; i++) {
        rank.song = select_song(app, i);
        if (!rank.song)
            continue;
        if (from_cover)
            done += image_from_cover(&rank) == 0;
        else
            done += image_import(&rank, sel->image) == 0;
    }
    image_cache_clear(app);
    snprintf(sel->msg, sizeof(sel->msg), "Image changée pour %d titre(s) "
        "sur %d.", done, sel->nb);
}

/*
** the files are removed (stats kept); exclude = 1 also keeps the video
** to never download it again (a song not from a video is left alone)
*/
void bulk_remove(app_t *app, int exclude)
{
    selection_t *sel = &app->libscene->sel;
    char id[VIDEO_ID];
    song_t *song = NULL;
    int done = 0;

    for (int i = 0; i < sel->nb; i++) {
        song = select_song(app, i);
        if (!song || (exclude && !song_video_id(app->lib, song, id)))
            continue;
        if (exclude)
            excluded_add(app->lib, id, song_title(song));
        song_remove_files(app->lib, song);
        done++;
    }
    db_save_excluded(app->lib);
    library_drop_missing(app);
    list_refresh(app);
    select_all(app, 0);
    snprintf(sel->msg, sizeof(sel->msg), "%d titre(s) %s.", done,
        exclude ? "exclu(s)" : "supprimé(s)");
}
