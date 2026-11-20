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
    #define BOARD_RESULTS 6

typedef struct app_s app_t;

typedef enum scene_id_e {
    HOME,
    LIBRARY,
    EDIT,
    BATTLE,
    RANKING,
    PLAYLIST,
    BOARDS,
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

/*
** playlist_id : tab shown (0 = "Tous les titres"), view / nb_view : its
** present songs, titles : one text per song of the view
** shown : song written in the bottom bar (has_shown = 0 before the 1st)
*/
typedef struct libscene_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    ui_t *ui;
    sfRectangleShape *panel;
    sfRectangleShape *bar;
    sfRectangleShape *progress_bg;
    sfRectangleShape *progress;
    int playlist_id;
    song_t **view;
    int nb_view;
    sfText **titles;
    sfText *now_title;
    sfText *now_artist;
    sfText *time;
    sfText *hint;
    int scroll;
    int rows;
    song_t *shown;
    int has_shown;
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
    FOCUS_SEARCH,
    FOCUS_IMAGE
} edit_focus_t;

/*
** field where the path of an image is pasted, and the result of the
** last import
*/
typedef struct image_input_s {
    char path[NAME_LEN];
    char msg[96];
    int focused;
} image_input_t;

/*
** title / artists / nb_artists : working copy, saved with "Enregistrer"
** input : "add an artist" field, suggest : existing artists matching it
** proposal : title and artists guessed from the ID3 title
** search / results / target : choice of the song to merge into
** artist : artist shown by the artist page
** image : import of an image for the song or the artist shown
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
    image_input_t image;
} edit_t;

/*
** pool : songs or artists of the arena (app->arena), rebuilt when the
** ranking or the kind changes; pair : the two shown, has_pair : 0 if
** the pool holds fewer than two
*/
typedef struct battle_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    pool_t pool;
    rank_t pair[2];
    int has_pair;
} battle_t;

typedef enum ranking_tab_e {
    TAB_TIERS,
    TAB_BOARD
} ranking_tab_t;

/*
** ranks : the songs or artists of the arena (app->arena) having at
** least one battle in it, best first (rebuilt when the arena changes)
*/
typedef struct ranking_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    ranking_tab_t tab;
    rank_t *ranks;
    int nb;
    int scroll;
} ranking_t;

/*
** edition of a playlist (app->edit_playlist, NULL = a new one)
** found : present songs matching the search, each one can be checked
** confirm : 1 while asking before removing the playlist
*/
typedef struct pledit_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    playlist_t *playlist;
    char name[NAME_LEN];
    char search[NAME_LEN];
    int focus;
    int confirm;
    song_t **found;
    int nb_found;
    int scroll;
} pledit_t;

typedef enum boards_mode_e {
    BOARDS_LIST,
    BOARDS_EDIT,
    BOARDS_CONFIRM
} boards_mode_t;

/*
** management of the rankings: list, then edition of one (board)
** name : name being typed, applied when leaving the edition
** results : rules that can be added for what is typed in search
** members : number of songs in the ranking
*/
typedef struct boards_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    boards_mode_t mode;
    board_t *board;
    char name[NAME_LEN];
    char search[NAME_LEN];
    int focus;
    rule_t results[BOARD_RESULTS];
    int nb_results;
    int members;
} boards_t;

#endif
