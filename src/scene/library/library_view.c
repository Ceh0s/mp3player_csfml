/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** songs of the tab shown in the library (a playlist or every song),
** filtered by the search
*/

#include "mp3player.h"

void list_free(app_t *app)
{
    libscene_t *scene = app->libscene;

    free(scene->view);
    scene->view = NULL;
    scene->nb_view = 0;
}

/*
** (re)builds the songs of the tab scene->playlist_id;
** nb_total counts its present songs before the search
*/
int list_create(app_t *app)
{
    libscene_t *scene = app->libscene;
    playlist_t *playlist = playlist_by_id(app->lib, scene->playlist_id);

    list_free(app);
    if (!playlist)
        scene->playlist_id = 0;
    scene->scroll = 0;
    scene->view = malloc(sizeof(song_t *) * (app->lib->nb + 1));
    if (!scene->view)
        return 84;
    scene->nb_view = playlist_sorted_view(app->lib, playlist, scene->view);
    scene->nb_total = scene->nb_view;
    scene->nb_view = view_filter(scene->view, scene->nb_view,
        app->search.text);
    return 0;
}
