/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** songs of the tab shown in the library (a playlist or every song) and
** the text of each of their rows
*/

#include "mp3player.h"

static char *row_label(song_t const *song)
{
    char const *title = song_title(song);
    char *label = malloc(strlen(title) + 4
        + (song->artists ? strlen(song->artists) : 0));

    if (!label)
        return NULL;
    strcpy(label, title);
    if (song->artists) {
        strcat(label, " - ");
        strcat(label, song->artists);
    }
    return label;
}

void list_free(app_t *app)
{
    libscene_t *scene = app->libscene;

    for (int i = 0; scene->titles && scene->titles[i]; i++)
        sfText_destroy(scene->titles[i]);
    free(scene->titles);
    free(scene->view);
    scene->titles = NULL;
    scene->view = NULL;
    scene->nb_view = 0;
}

static int create_titles(app_t *app, libscene_t *scene)
{
    char *label = NULL;

    scene->titles = calloc(scene->nb_view + 1, sizeof(sfText *));
    if (!scene->titles)
        return 84;
    for (int i = 0; i < scene->nb_view; i++) {
        scene->titles[i] = text_create(app, 36, LIST_X, LIST_Y);
        label = row_label(scene->view[i]);
        if (!scene->titles[i] || !label) {
            free(label);
            return 84;
        }
        text_set(scene->titles[i], label);
        free(label);
    }
    return 0;
}

/*
** (re)builds the songs and texts of the tab scene->playlist_id
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
    return create_titles(app, scene);
}
