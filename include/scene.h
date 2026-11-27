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
    STATS,
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
** present songs (rows drawn with widget_song)
** shown : song written in the bottom bar (has_shown = 0 before the 1st)
** drag_row : row held by the mouse in the custom order (-1 if none),
** press : where it was pressed, dragging : 1 once the mouse moved
** list : its scroll in pixels and its slider (scroll.c, library_scroll.c);
** height : pixels of the list on the screen
** selecting : 1 in the selection mode (a click checks a song), sel : it
** sort_open : 1 while the list of the sort criteria is open
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
    int nb_total;
    sfText *now_title;
    sfText *now_artist;
    sfText *time;
    sfText *hint;
    scroll_t list;
    int height;
    song_t *shown;
    int has_shown;
    int drag_row;
    sfVector2f press;
    int dragging;
    int selecting;
    selection_t sel;
    int sort_open;
} libscene_t;

/*
** what the library shows again when coming back from another screen, if
** its tab (playlist_id) is the same: place in the list and search
*/
typedef struct library_keep_s {
    int playlist_id;
    float scroll;
    char search[NAME_LEN];
} library_keep_t;

/*
** stats of a song or an artist, in a window over its page (stat_panel.c):
** open : 1 while shown, choose : 1 to show every stat with its box,
** subject : whose stats (song resolved), values : of each stat (0 to 100,
** -1 = none, -2 = not for this kind), rows / nb : stats drawn at the
** last frame, in their order
*/
typedef struct statpanel_s {
    int open;
    int choose;
    rank_t subject;
    int values[NB_STATS];
    int rows[NB_STATS];
    int nb;
} statpanel_t;

typedef enum edit_mode_e {
    EDIT_MAIN,
    EDIT_SEARCH,
    EDIT_CONFIRM,
    EDIT_ARTIST,
    EDIT_TITLES,
    EDIT_NOTES
} edit_mode_t;

typedef enum edit_focus_e {
    FOCUS_TITLE,
    FOCUS_ARTIST,
    FOCUS_SEARCH,
    FOCUS_IMAGE,
    FOCUS_RENAME,
    FOCUS_FOUND,
    FOCUS_FOUND_DATE,
    FOCUS_PERIOD
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
** working copy of the page "Découverte" (edit_notes.c), saved with
** "Enregistrer"; found / date : the two free fields
** rows : the stays of the song in all the playlists: old : as they were
** (old.song = 0 for a new one), from / to : dates typed, first : see
** period_t (the one of each playlist is on all its rows), removed : 1
** once "Retirer" was clicked
** playlist : the one whose stays are shown; lists / periods : scroll of
** the playlists and of its stays (scroll.c); row / side : date field receiving the
** keys (side 0 = from, 1 = to); msg : why it was not saved
*/
    #define PERIOD_ROWS 64
    #define PERIOD_DATE_LEN 11

typedef struct period_row_s {
    period_t old;
    int playlist;
    char from[PERIOD_DATE_LEN];
    char to[PERIOD_DATE_LEN];
    int removed;
} period_row_t;

typedef struct notes_edit_s {
    char found[NAME_LEN];
    char date[NAME_LEN];
    period_row_t rows[PERIOD_ROWS];
    int nb;
    int playlist;
    scroll_t lists;
    scroll_t periods;
    int row;
    int side;
    char msg[96];
} notes_edit_t;

/*
** title / artists / nb_artists : working copy, saved with "Enregistrer"
** input : "add an artist" field, suggest : existing artists matching it
** show_hidden : 1 to list the hidden suggestions too (EDIT_TITLES page)
** search / results / target : choice of the song to merge into
** artist : artist shown by the artist page, titles : scroll of its songs
** stats : window of the stats of the song or of the artist shown
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
    char rename[NAME_LEN];
    artist_t *renamed;
    scroll_t titles;
    statpanel_t stats;
    int stat_values[NB_STATS];
    scroll_t summary;
    notes_edit_t notes;
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
    TAB_TIES,
    TAB_HISTORY
} ranking_tab_t;

/*
** ranks : the songs or artists of the arena (app->arena), best first
** (rebuilt when the arena changes), nb of them; tiers : tier of each
** one; order : the same in the order chosen between ex aequo
** ("Départage" tab, drag_row : row held, -1 if none)
** reset : 1 while the reset window is open, confirm : what is typed in it
** open_tier : tier shown alone on the whole table (-1 = all of them)
** list : scroll of the tab shown (scroll.c; leaderboard, ties, history,
** tier shown alone)
** hover : song or artist under the mouse at the last drawing (both NULL
** if none), opened by a right click (ranking_open.c)
** undo_left : battles of the history that may still be undone, undo_ask :
** 1 once "Annuler" was clicked (a second click does it)
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
    scroll_t list;
    int drag_row;
    int dragging;
    sfVector2f press;
    int reset;
    char confirm[16];
    int open_tier;
    rank_t hover;
    history_t history;
    int history_dirty;
    int undo_left;
    int undo_ask;
} ranking_t;

/*
** what the rankings screen shows again when coming back from the page
** of a song or an artist (active = 1 then)
*/
/*
** list of the categories opened by the category bar (category_list.c),
** shared by the battle and ranking screens: open : 1 while it is shown,
** nb : its rows at the last drawing, total : 1 if TOTAL is one of them;
** edit : stat + 1 shown in the window of a category (category_edit.c; 0
** if none, CAT_NEW for a new one, also opened from the stats of a page),
** name / text / flaw : its working copy, field : 1 if the name gets the
** keys (else the description), confirm : 1 once "Supprimer" was clicked,
** refused : 1 if the name typed is empty or taken
*/
typedef struct catlist_s {
    int open;
    int nb;
    int total;
    int edit;
    char name[CAT_NAME_LEN];
    char text[DESC_LEN];
    int flaw;
    int board_flaw;
    int drag;
    int field;
    int confirm;
    int refused;
} catlist_t;

typedef struct ranking_keep_s {
    int active;
    int tab;
    float scroll;
    int open_tier;
} ranking_keep_t;

/*
** edition of a playlist (app->edit_playlist, NULL = a new one)
** found : present songs matching the search, each one can be checked
** confirm : 1 while asking before removing the playlist
** takeout : path of a Takeout file (dates of the songs), takeout_msg :
** result of its import, takeout_ask : 1 once it was refused because most
** of its videos are not in the playlist (a second click imports it)
** focus : field receiving the keys (0 name, 1 search, 2 takeout)
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
    scroll_t list;
    char takeout[NAME_LEN];
    char takeout_msg[160];
    int takeout_ask;
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
** list : scroll of the rankings, or of the rules of the one edited
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
    scroll_t list;
} boards_t;

/*
** url : addresses being typed, separated by ; (the download itself is
** app->download), URLS_LEN leaves room for about 90 links
** min / max : allowed duration typed ("m:ss" or minutes, empty = no
** limit), focus : field receiving the keys (DL_FIELD_...)
** targets : local playlists chosen to receive the songs
** link / absent : options of a single playlist (dl_options_t)
** popup : list shown over the screen (DL_POPUP_...), list : scroll of its
** rows (scroll.c), nb_rows : rows of it counted at the last drawing,
** confirm : song whose files are about to be removed
** (asked a second time), 0 if none
** archived : 1 to show the archived rows of "Refusés" / "Exclus",
** clear : 1 once "Vider la liste" was clicked (asked a second time),
** forget : row + 1 of the excluded song about to be forgotten, 0 if none
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
    DL_POPUP_REPORT,
    DL_POPUP_UNAVAILABLE,
    DL_POPUP_SPLITS
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
    scroll_t list;
    int nb_rows;
    int confirm;
    int archived;
    int clear;
    int forget;
} dlscene_t;

/*
** possible duplicates: the groups found by dup_find (list), then the
** sound of the songs in them is read, one song per tick (todo, done:
** the window keeps being drawn); ready once every group is compared
** auto_merged : songs merged on their own (dup_merge.c)
** rows : scroll of the items (the files of identical sound, then the
** groups; scroll.c), confirm : file of identical sound about to be removed + 1
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
    scroll_t rows;
    int confirm;
} dupscene_t;

/*
** a listening session of listens.log: date (seconds), raw id of the
** song, time really listened (ms), skip : 1 if it was skipped
*/
typedef struct listen_s {
    long date;
    int song_id;
    long ms;
    int skip;
} listen_t;

/*
** a row of the statistics: a song or an artist (the sum of its songs),
** time listened, sessions, skips, and its Elo in the category chosen
*/
typedef struct stat_row_s {
    song_t *song;
    artist_t *artist;
    long ms;
    int plays;
    int skips;
    double elo;
} stat_row_t;

/*
** choices of the statistics screen, kept during the session
** (app->stats_opt): playlist_id : 0 = every present song, artists : 1
** for the artists, elo : 0 = no Elo column, else category + 1 (TOTAL
** included), sort : STAT_SORT_..., ascending : 1 for the smallest first,
** from / to : dates typed (AAAA-MM-JJ, empty = no limit)
** scroll : first row shown again when coming back from a page
*/
    #define STAT_SORT_TIME 0
    #define STAT_SORT_PLAYS 1
    #define STAT_SORT_SKIPS 2
    #define STAT_SORT_ELO 3
    #define STAT_SORT_NAME 4
    #define NB_STAT_SORTS 5
    #define STAT_DATE_LEN 11

typedef struct stats_opt_s {
    int playlist_id;
    int artists;
    int elo;
    int sort;
    int ascending;
    char from[STAT_DATE_LEN];
    char to[STAT_DATE_LEN];
    float scroll;
} stats_opt_t;

/*
** statistics screen: log : listens.log read when the screen opens (nb_log
** sessions), rows : the leaderboard (nb of them), rebuilt when a choice
** changes (dirty), list : their scroll (scroll.c), focus : 0, or the date field
** receiving the keys (1 = from, 2 = to); hover : song or artist under
** the mouse at the last drawing, opened by a right click (page_open.c)
*/
typedef struct statscene_s {
    sfTexture *bg_texture;
    sfSprite *bg;
    sfRectangleShape *panel;
    listen_t *log;
    int nb_log;
    stat_row_t *rows;
    int nb;
    scroll_t list;
    int focus;
    int dirty;
    rank_t hover;
} statscene_t;

#endif
