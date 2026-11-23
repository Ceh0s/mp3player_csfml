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
    #include "download.h"

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
    DOWNLOAD,
    DUPLICATES,
    SETTINGS,
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

/*
** search field shared by the screens with a long list (one at a time,
** emptied when the screen changes): text typed, focus : 1 while it
** receives the keys, shown : 1 if the screen drew it at the last frame,
** rect : where, changed : 1 after a change (cleared by the screen)
*/
typedef struct search_s {
    char text[NAME_LEN];
    int focus;
    int shown;
    float rect[4];
    int changed;
} search_t;

typedef struct setscene_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
} setscene_t;

typedef struct home_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    ui_t *ui;
} home_t;

/*
** selection mode of the library: ids : the songs checked (resolved
** ids), nb of them, last : row clicked last (for Shift + click, -1 if
** none); open : 1 while the actions window is over the list
** playlist : playlist chosen for "Ajouter" / "Retirer", text : what is
** inserted in the titles, where : SEL_START / SEL_END / SEL_AFTER (after
** the number of characters typed in after), image : path of an image
** for all, focus : field receiving the keys (SEL_FOCUS_...), confirm :
** SEL_EXCLUDE or SEL_DELETE once clicked (a second click confirms),
** msg : result of the last action
*/
    #define SEL_START 0
    #define SEL_END 1
    #define SEL_AFTER 2
    #define SEL_FOCUS_NONE 0
    #define SEL_FOCUS_TEXT 1
    #define SEL_FOCUS_AFTER 2
    #define SEL_FOCUS_IMAGE 3
    #define SEL_EXCLUDE 1
    #define SEL_DELETE 2

typedef struct selection_s {
    int *ids;
    int nb;
    int last;
    int open;
    int playlist;
    char text[NAME_LEN];
    int where;
    char after[8];
    char image[NAME_LEN];
    int focus;
    int confirm;
    char msg[96];
} selection_t;

/*
** playlist_id : tab shown (0 = "Tous les titres"), view / nb_view : its
** present songs, titles : one text per song of the view
** shown : song written in the bottom bar (has_shown = 0 before the 1st)
** drag_row : row held by the mouse in the custom order (-1 if none),
** press : where it was pressed, dragging : 1 once the mouse moved,
** drag_wait : ticks before the list scrolls again while dragging
** selecting : 1 in the selection mode (a click checks a song), sel : it
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
    int drag_row;
    sfVector2f press;
    int dragging;
    int drag_wait;
    int selecting;
    selection_t sel;
} libscene_t;

typedef enum edit_mode_e {
    EDIT_MAIN,
    EDIT_SEARCH,
    EDIT_CONFIRM,
    EDIT_ARTIST,
    EDIT_TITLES
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
** show_hidden : 1 to list the hidden suggestions too (EDIT_TITLES page)
** search / results / target : choice of the song to merge into
** artist : artist shown by the artist page
** image : import of an image for the song or the artist shown
** exclude : EDIT_ASK_EXCLUDE or EDIT_ASK_DELETE once "Exclure" or
** "Supprimer du lecteur" was clicked (a second click confirms), 0 if none
*/
    #define EDIT_ASK_EXCLUDE 1
    #define EDIT_ASK_DELETE 2

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
    int show_hidden;
    char search[NAME_LEN];
    song_t *results[EDIT_RESULTS];
    int nb_results;
    song_t *target;
    artist_t *artist;
    image_input_t image;
    int exclude;
} edit_t;

/*
** pool : songs or artists of the arena (app->arena), rebuilt when the
** ranking or the kind changes; pair : the two shown, has_pair : 0 if
** the pool holds fewer than two; focus : side chosen with the arrows
** (-1 = none), voted with Enter
*/
typedef struct battle_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    pool_t pool;
    rank_t pair[2];
    int has_pair;
    int focus;
} battle_t;

typedef enum ranking_tab_e {
    TAB_TIERS,
    TAB_BOARD,
    TAB_TIES
} ranking_tab_t;

/*
** ranks : the songs or artists of the arena (app->arena), best first
** (rebuilt when the arena changes), nb of them; tiers : tier of each
** one; order : the same in the order chosen between ex aequo
** ("Départage" tab, drag_row : row held, -1 if none)
** reset : 1 while the reset window is open, confirm : what is typed in it
*/
typedef struct ranking_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    ranking_tab_t tab;
    rank_t *ranks;
    rank_t *order;
    int *tiers;
    int nb;
    int scroll;
    int drag_row;
    int dragging;
    sfVector2f press;
    int reset;
    char confirm[16];
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

/*
** url : addresses being typed, separated by ; (the download itself is
** app->download), URLS_LEN leaves room for about 90 links
** min / max : allowed duration typed ("m:ss" or minutes, empty = no
** limit), focus : field receiving the keys (DL_FIELD_...)
** targets : local playlists chosen to receive the songs
** link / absent : options of a single playlist (dl_options_t)
** popup : list shown over the screen (DL_POPUP_...), scroll : its
** first row shown, confirm : song whose files are about to be removed
** (asked a second time), 0 if none
*/
    #define URLS_LEN 4096
    #define DURATION_LEN 10

typedef enum dl_field_e {
    DL_FIELD_URL,
    DL_FIELD_MIN,
    DL_FIELD_MAX
} dl_field_t;

typedef enum dl_popup_e {
    DL_POPUP_NONE,
    DL_POPUP_FLAGGED,
    DL_POPUP_REFUSED,
    DL_POPUP_ABSENT,
    DL_POPUP_EXCLUDED,
    DL_POPUP_REPORT
} dl_popup_t;

typedef struct dlscene_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    char url[URLS_LEN];
    char min[DURATION_LEN];
    char max[DURATION_LEN];
    dl_field_t focus;
    int targets[DL_TARGETS];
    int nb_targets;
    int link;
    int absent;
    dl_popup_t popup;
    int scroll;
    int confirm;
} dlscene_t;

/*
** possible duplicates: the groups found by dup_find (list), then the
** sound of the songs in them is read, one song per tick (todo, done:
** the window keeps being drawn); ready once every group is compared
** auto_merged : songs merged on their own (dup_merge.c)
** scroll : first item shown (the files of identical sound, then the
** groups), confirm : file of identical sound about to be removed + 1
** (asked a second time), 0 if none
*/
typedef struct dupscene_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    dup_list_t list;
    song_t **todo;
    int nb_todo;
    int done;
    int ready;
    int auto_merged;
    int scroll;
    int confirm;
} dupscene_t;

#endif
