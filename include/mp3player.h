/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** main header: constants and prototypes
*/

#ifndef MP3PLAYER_H_
    #define MP3PLAYER_H_

    #define WINDOW_TITLE "MP3 Player"
    #define VIEW_W 1920
    #define VIEW_H 1080
    #define WINDOWED_W 1280
    #define WINDOWED_H 720
    #define TICK_MS 33
    #define AUTOSAVE_US 30000000

    #define FONT_PATH "assets/font.ttf"
    #define ATLAS_PATH "assets/atlas.png"
    #define BG_PATH "assets/background.jpg"
    #define BG_BLUR_PATH "assets/background_blurred.jpg"
    #define ATLAS_CELL 256

    #define MUSIC_DIR "music"
    #define DATA_DIR "data"
    #define SONGS_DB "data/songs.tsv"
    #define FPRINTS_DB "data/fingerprints.tsv"
    #define ARTISTS_DB "data/artists.tsv"
    #define LINKS_DB "data/song_artists.tsv"
    #define LISTENS_LOG "data/listens.log"
    #define EDITS_LOG "data/edits.log"
    #define MERGES_LOG "data/merges.log"
    #define ELO_DB "data/elo.tsv"
    #define BATTLES_LOG "data/battles.log"
    #define PLAYLISTS_DB "data/playlists.tsv"
    #define PLAYLIST_SONGS_DB "data/playlist_songs.tsv"
    #define BOARDS_DB "data/boards.tsv"
    #define RULES_DB "data/board_rules.tsv"
    #define SONGS_COLS_1 "id\tmerged_into\tfile\ttitle\talbum\tgenre\tyear"
    #define SONGS_COLS_2 "\tsrc_title\tsrc_artists\tduration_ms\tfirst_seen"
    #define SONGS_COLS_3 "\tlast_seen\tpresent\tlisten_ms\tplay_count"
    #define SONGS_COLS_4 "\tskip_count"
    #define SONGS_COLUMNS SONGS_COLS_1 SONGS_COLS_2 SONGS_COLS_3 SONGS_COLS_4
    #define FPRINTS_COLUMNS "hash\tsong_id"
    #define ARTISTS_COLUMNS "id\tname"
    #define LINKS_COLUMNS "song_id\tartist_id\trole"
    #define LISTENS_COLUMNS "timestamp\tsong_id\tlisten_ms\tend"
    #define EDITS_COLUMNS "timestamp\tsong_id\tfield\tbefore\tafter"
    #define MERGES_COLUMNS "timestamp\taction\tsource_id\ttarget_id"
    #define ELO_COLS_1 "kind\tboard\tsubject\tcategory"
    #define ELO_COLUMNS ELO_COLS_1 "\trating\tbattles\twins"
    #define BATTLES_COLS_1 "timestamp\tkind\tboard\tcategory\twinner\tloser"
    #define BATTLES_COLUMNS BATTLES_COLS_1 "\twinner_before\tloser_before"
    #define PLAYLISTS_COLUMNS "id\tname"
    #define PLAYLIST_SONGS_COLUMNS "playlist_id\tsong_id\tposition"
    #define BOARDS_COLUMNS "id\tname"
    #define RULES_COLUMNS "board_id\tmode\ttype\ttarget"
    #define TSV_MAX_FIELDS 20
    #define V1_FIELDS 14
    #define V2_FIELDS 16
    #define FPRINT_BYTES 262144

    #define LIST_X 130
    #define LIST_Y 196
    #define TABS_Y 124
    #define TABS_RIGHT 1530
    #define LIST_STEP 52
    #define PANEL_H 790
    #define BOARD_BAR_Y 190
    #define CAT_BAR_Y 260
    #define CONTENT_Y 350
    #define BATTLE_CARD_Y 340
    #define BATTLE_CARD_H 420
    #define CONTENT_BOTTOM 820
    #define BOARD_STEP 46
    #define PL_ROWS_Y 330
    #define PL_ROW_STEP 52
    #define BOARDS_ROWS_Y 330

    #include "app.h"
    #include <stdlib.h>
    #include <unistd.h>
    #include <stddef.h>
    #include <string.h>
    #include <stdio.h>
    #include <time.h>
    #include <SFML/Graphics.h>
    #include <SFML/System.h>
    #include <SFML/Window.h>
    #include <SFML/Audio.h>

extern const scene_t scenes[NB_SCENES];
extern const char *const elo_categories[NB_CATEGORIES];

// src/core : app, main loop, window, view, signals
app_t *app_create(void);
int app_destroy(app_t *app, int rtn_value);
int loop_run(app_t *app);
int window_create(app_t *app);
void window_apply_toggle(app_t *app);
int view_create(app_t *app);
void view_update(app_t *app);
void view_mouse(app_t *app);
int signals_block(void);
int signals_pending(void);

// src/scene
int scene_switch(app_t *app);
void scene_open(app_t *app, scene_id_t id);
void scene_back(app_t *app);

// src/scene/home
int home_init(app_t *app);
void home_event(app_t *app, sfEvent *event);
void home_draw(app_t *app);
void home_layout(app_t *app);
void home_destroy(app_t *app);

// src/scene/library
int libscene_init(app_t *app);
void libscene_event(app_t *app, sfEvent *event);
void libscene_update(app_t *app);
void libscene_draw(app_t *app);
void libscene_layout(app_t *app);
void libscene_destroy(app_t *app);
int list_create(app_t *app);
int list_click(app_t *app);
void list_scroll(app_t *app, float delta);
void list_update(app_t *app);
void list_draw(app_t *app);
int bar_create(app_t *app);
int bar_seek(app_t *app);
void bar_update(app_t *app);
void bar_draw(app_t *app);
void bar_layout(app_t *app);
void list_free(app_t *app);
void library_tabs(app_t *app);

// src/scene/edit
int edit_init(app_t *app);
void edit_event(app_t *app, sfEvent *event);
void edit_draw(app_t *app);
void edit_layout(app_t *app);
void edit_destroy(app_t *app);
void edit_main(app_t *app);
void edit_search(app_t *app);
void edit_confirm(app_t *app);
void edit_artist(app_t *app);
void edit_artist_row(app_t *app, edit_t *edit, int i);
void edit_text_entered(app_t *app, sfUint32 code);
void edit_add_artist(app_t *app, char const *name, role_t role);
void edit_refresh_lists(app_t *app);
void edit_propose(app_t *app);
void edit_apply_proposal(app_t *app);

// src/scene/battle
int battle_init(app_t *app);
void battle_event(app_t *app, sfEvent *event);
void battle_draw(app_t *app);
void battle_layout(app_t *app);
void battle_destroy(app_t *app);
void battle_view(app_t *app);
void battle_next(app_t *app);
void battle_vote(app_t *app, int side);
void battle_reload(app_t *app);

// src/scene/ranking
int ranking_init(app_t *app);
void ranking_event(app_t *app, sfEvent *event);
void ranking_draw(app_t *app);
void ranking_layout(app_t *app);
void ranking_destroy(app_t *app);
void ranking_build(app_t *app);
void ranking_view(app_t *app);
void ranking_tiers(app_t *app);
void ranking_board(app_t *app);
void ranking_scroll(app_t *app, float delta);

// src/scene/playlist
int pledit_init(app_t *app);
void pledit_event(app_t *app, sfEvent *event);
void pledit_draw(app_t *app);
void pledit_layout(app_t *app);
void pledit_destroy(app_t *app);
void pledit_search(app_t *app);
void pledit_scroll(app_t *app, float delta);
void pledit_view(app_t *app);

// src/scene/boards
int boards_init(app_t *app);
void boards_event(app_t *app, sfEvent *event);
void boards_draw(app_t *app);
void boards_layout(app_t *app);
void boards_destroy(app_t *app);
void boards_list(app_t *app);
void boards_open(app_t *app, board_t *board);
void boards_leave(app_t *app);
void boards_edit(app_t *app);
void boards_confirm(app_t *app);
void boards_count(app_t *app);
void boards_text(app_t *app, sfUint32 code);
void boards_add_column(app_t *app);

// src/scene/pages : pieces of screens usable by several scenes
void artist_page(app_t *app, artist_t *artist, int *back);
int category_bar(app_t *app, int *category, int nb, int random);
int board_bar(app_t *app, arena_t *arena);

// src/ui
ui_t *ui_create(sfTexture *texture, ui_def_t const *defs, int nb);
void ui_draw(app_t *app, ui_t *ui);
void ui_show(ui_t *ui, int id, int visible);
void ui_layout(ui_t *ui, float extra_h);
void ui_destroy(ui_t *ui);
int ui_hit(app_t *app, ui_t *ui);
int mouse_in_rect(app_t *app, float const *rect);
sfText *text_create(app_t *app, unsigned int size, float x, float y);
void text_set(sfText *text, char const *str);
sfSprite *background_create(char const *path, sfTexture **texture);
void background_layout(app_t *app, sfSprite *sprite);
sfRectangleShape *rect_create(float const *rect, sfColor const *color);
widgets_t *widgets_create(app_t *app);
void widgets_destroy(widgets_t *wg);
void widget_text(app_t *app, char const *str, float const *pos,
    sfColor const *color);
int widget_button(app_t *app, float const *rect, char const *label);
int widget_field(app_t *app, float const *rect, char const *value,
    int focused);
void widget_rect(app_t *app, float const *rect, sfColor const *color);
float widget_text_width(app_t *app, char const *str, unsigned int size);

// src/audio
void player_play(app_t *app, int index);
void player_toggle(app_t *app);
void player_skip(app_t *app, int direction);
void player_stop(app_t *app, end_reason_t reason);
void player_update(app_t *app);
void player_resync(app_t *app);
void player_queue(app_t *app, int playlist_id);
void listen_log(song_t const *song, long ms, end_reason_t reason);

// src/library
library_t *library_load(void);
int library_refresh(library_t *lib);
void library_destroy(library_t *lib);

// src/library/file : files on disk
long id3_synchsafe(unsigned char const *bytes);
long mp3_audio_offset(unsigned char const *head);
int mp3_is_valid(char const *path);
char *fingerprint_file(char const *path);
song_t *scan_music(char const *dir);

// src/library/id3 : tags
void id3_read(song_t *song, char const *path);
void id3_parse_frames(song_t *song, unsigned char const *tag, long size,
    int version);
char *id3_decode_text(unsigned char const *data, long size);

// src/library/songs
song_t *song_create(void);
void song_destroy(song_t *song);
void song_list_destroy(song_t *song);
char const *song_title(song_t const *song);
char *song_make_key(song_t const *song);
song_t *song_by_id(library_t *lib, int id);
song_t *song_resolve(library_t *lib, song_t *song);
long song_total_listen(library_t *lib, song_t const *song);
fprint_t *fprint_find(library_t *lib, char const *hash);
fprint_t *fprint_add(library_t *lib, char const *hash, int song_id);
void song_set_title(song_t *song, char const *title);
void song_set_artists(library_t *lib, song_t *song,
    artist_role_t const *list, int nb);
void library_changed(app_t *app);
int song_merge(app_t *app, song_t *source, song_t *target);
void song_split(app_t *app, song_t *alias);
int merge_warnings(song_t const *src, song_t const *dst,
    char warn[EDIT_WARNINGS][128]);

// src/library/artists
artist_t *artist_find(library_t *lib, char const *name);
artist_t *artist_get(library_t *lib, char const *name);
artist_t *artist_by_id(library_t *lib, int id);
int link_add(library_t *lib, int song_id, int artist_id, role_t role);
void links_remove_song(library_t *lib, int song_id);
void artists_from_source(library_t *lib, song_t *song);
void song_build_artists(library_t *lib, song_t *song);
long artist_listen_ms(library_t *lib, int artist_id);
char const *role_name(role_t role);
role_t role_from_name(char const *name);
int song_has_role(library_t *lib, int song_id, role_t role);
void artist_stats(library_t *lib, int artist_id, artist_stats_t *stats);
artist_part_t song_part(library_t *lib, link_t const *link);

// src/library/playlists
playlist_t *playlist_add(library_t *lib, int id, char const *name);
playlist_t *playlist_by_id(library_t *lib, int id);
void playlist_remove(library_t *lib, playlist_t *playlist);
int playlist_has_song(library_t *lib, playlist_t const *playlist,
    song_t const *song);
int playlist_toggle(library_t *lib, playlist_t *playlist, song_t *song);
int playlist_view(library_t *lib, playlist_t const *playlist,
    song_t **view);

// src/library/boards : rankings and their rules
board_t *board_add(library_t *lib, int id, char const *name);
board_t *board_by_id(library_t *lib, int id);
int board_general_create(library_t *lib);
void board_remove(library_t *lib, board_t *board);
int rule_add(board_t *board, int include, rule_type_t type, int target);
void rule_remove(board_t *board, rule_t *rule);
void rule_text(library_t *lib, rule_t const *rule, char *buf, int size);
char const *rule_type_name(rule_type_t type);
void rules_forget(library_t *lib, rule_type_t type, int target);
int board_has_song(library_t *lib, int board_id, song_t *song);

// src/elo : ratings, without any display
int category_find(char const *name);
char const *category_name(int category);
int kind_find(char const *name);
char const *kind_name(elo_kind_t kind);
elo_t *elo_find(library_t *lib, arena_t const *arena, int subject);
elo_t *elo_get(library_t *lib, arena_t const *arena, int subject);
void elo_battle(library_t *lib, arena_t const *arena, int winner,
    int loser);
void elo_clear(library_t *lib);
int rank_id(rank_t const *rank);
char const *rank_name(rank_t const *rank);
int subject_resolve(library_t *lib, elo_kind_t kind, int id);
void elo_score(library_t *lib, arena_t const *arena, rank_t *out);
int elo_pick_pair(library_t *lib, arena_t const *arena, pool_t *pool,
    rank_t *pair);
int pool_build(library_t *lib, arena_t const *arena, pool_t *pool);

// src/data : save files
int tsv_read(char const *path, tsv_fn_t fn, library_t *lib);
FILE *tsv_create(char const *path, char const *columns);
int tsv_commit(FILE *file, char const *path);
void tsv_touch(char const *path, char const *columns);
void db_load_all(library_t *lib);
int db_save_all(library_t *lib);
void db_parse_song(library_t *lib, char **fields, int nb);
int db_save_songs(library_t *lib);
void db_parse_fprint(library_t *lib, char **fields, int nb);
void db_parse_artist(library_t *lib, char **fields, int nb);
void db_parse_link(library_t *lib, char **fields, int nb);
int db_save_tables(library_t *lib);
void db_merge(library_t *lib, song_t *scanned);
void db_migrate(library_t *lib);
void db_migrate_battles(void);
void db_parse_playlist(library_t *lib, char **fields, int nb);
void db_parse_playlist_song(library_t *lib, char **fields, int nb);
int db_save_playlists(library_t *lib);
void db_parse_board(library_t *lib, char **fields, int nb);
void db_parse_rule(library_t *lib, char **fields, int nb);
int db_save_boards(library_t *lib);
void log_line(char const *path, char const *columns, char const *line);
void db_load_elo(library_t *lib);
int db_save_elo(library_t *lib);
void battle_record(library_t *lib, arena_t const *arena,
    rank_t const *winner, rank_t const *loser);

// src/utils
int utf8_encode(char *out, unsigned int code);
sfUint32 *utf8_to_utf32(char const *str);
char *str_lower(char const *str);
char *str_or_null(char const *str);
int str_contains(char const *hay, char const *needle);
int split_tab(char *line, char **fields, int max);
int text_input(char *buf, sfUint32 code);
void format_time(char *buf, size_t size, long ms);

#endif
