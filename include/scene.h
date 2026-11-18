/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scenes (screens) of the application
*/

#ifndef SCENE_H_
    #define SCENE_H_

    #include <SFML/Graphics.h>
    #include "ui.h"
    #include "song.h"

    #define EDIT_ARTISTS 6
    #define EDIT_SUGGEST 5
    #define EDIT_RESULTS 10
    #define EDIT_WARNINGS 4

typedef struct app_s app_t;

typedef enum scene_id_e {
    HOME,
    LIBRARY,
    EDIT,
    NB_SCENES
} scene_id_t;

typedef struct scene_s {
    scene_id_t id;
    int (*init)(app_t *app);
    void (*event)(app_t *app, sfEvent *event);
    void (*update)(app_t *app);
    void (*draw)(app_t *app);
    void (*layout)(app_t *app);
    void (*destroy)(app_t *app);
} scene_t;

typedef struct home_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    ui_t *ui;
} home_t;

typedef struct libscene_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    ui_t *ui;
    sfRectangleShape *panel;
    sfRectangleShape *bar;
    sfRectangleShape *progress_bg;
    sfRectangleShape *progress;
    sfText **titles;
    sfText *now_title;
    sfText *now_artist;
    sfText *time;
    sfText *hint;
    int scroll;
    int rows;
    int shown;
} libscene_t;

typedef enum edit_mode_e {
    EDIT_MAIN,
    EDIT_SEARCH,
    EDIT_CONFIRM,
    EDIT_ARTIST
} edit_mode_t;

typedef enum edit_focus_e {
    FOCUS_TITLE,
    FOCUS_ARTIST,
    FOCUS_SEARCH
} edit_focus_t;

/*
** title / artists / nb_artists : working copy, saved with "Enregistrer"
** input : "add an artist" field, suggest : existing artists matching it
** proposal : title and artists guessed from the ID3 title
** search / results / target : choice of the song to merge into
** artist : artist shown by the artist page
*/
typedef struct edit_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    song_t *song;
    edit_mode_t mode;
    edit_focus_t focus;
    char title[NAME_LEN];
    artist_role_t artists[EDIT_ARTISTS];
    int nb_artists;
    char input[NAME_LEN];
    artist_t *suggest[EDIT_SUGGEST];
    int nb_suggest;
    int has_proposal;
    char proposal_title[NAME_LEN];
    artist_role_t proposal[EDIT_ARTISTS];
    int nb_proposal;
    char search[NAME_LEN];
    song_t *results[EDIT_RESULTS];
    int nb_results;
    song_t *target;
    artist_t *artist;
} edit_t;

#endif
