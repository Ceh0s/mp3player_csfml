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
    #define NO_COVER_PATH "assets/no_cover.png"
    #define ATLAS_CELL 256
    #define ATLAS_COLS 10
    #define ICON_CELL(icon, filled) ((icon) + (filled) * NB_ICONS)
    #define ICON_TX(icon, filled) (ICON_CELL(icon, filled) % ATLAS_COLS)
    #define ICON_TY(icon, filled) (ICON_CELL(icon, filled) / ATLAS_COLS)
    #define ICON_X(icon, filled) (ICON_TX(icon, filled) * ATLAS_CELL)
    #define ICON_Y(icon, filled) (ICON_TY(icon, filled) * ATLAS_CELL)
    #define VOLUME_STEP 10
    #define SEEK_MS 10000

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
    #define IMAGES_DIR "data/images"
    #define IMAGE_BIG 400
    #define IMAGE_SMALL 120
    #define COVER_CHECKED IMAGES_DIR "/.covers_checked"
    #define COVER_SAME 8
    #define PATH_LEN 256
    #define PLAYLISTS_DB "data/playlists.tsv"
    #define PLAYLIST_SONGS_DB "data/playlist_songs.tsv"
    #define SORTS_DB "data/playlist_sorts.tsv"
    #define CUSTOM_DB "data/custom_orders.tsv"
    #define TIES_DB "data/tie_orders.tsv"
    #define LIKES_DB "data/likes.tsv"
    #define EXCLUDED_DB "data/excluded.tsv"
    #define UNAVAILABLE_DB "data/unavailable.tsv"
    #define SUGGEST_DB "data/suggestions.tsv"
    #define SPLITS_DB "data/artist_splits.tsv"
    #define SPLITS_COLUMNS "name\tchoice"
    #define SPLIT_MAX 8
    #define SETTINGS_DB "data/settings.tsv"
    #define DOWNLOADS_LOG "data/downloads.log"
    #define BOARDS_DB "data/boards.tsv"
    #define RULES_DB "data/board_rules.tsv"
    #define REJECTED_DB "data/rejected.tsv"
    #define SOURCES_DB "data/playlist_sources.tsv"
    #define ENVELOPES_DB "data/envelopes.tsv"
    #define NOT_DUPS_DB "data/not_duplicates.tsv"
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
    #define PLAYLISTS_COLUMNS "id\tname\tauto"
    #define LIKES_COLUMNS "kind\tid\tvalue"
    #define EXCLUDED_COLUMNS "video_id\ttimestamp\ttitle\tarchived"
    #define UNAVAILABLE_COLUMNS "video_id\ttimestamp\ttitle\treason"
    #define SUGGEST_COLUMNS "song_id\trule\tremoved\tstate"
    #define SETTINGS_COLUMNS "key\tvalue"
    #define DOWNLOADS_COLUMNS "timestamp\tstatus\ttitle\tdetail"
    #define PLAYLIST_SONGS_COLUMNS "playlist_id\tsong_id\tposition"
    #define SORTS_COLUMNS "playlist_id\tkey\tdirection"
    #define CUSTOM_COLUMNS "playlist_id\tsong_id\tposition"
    #define BOARDS_COLUMNS "id\tname"
    #define RULES_COLUMNS "board_id\tmode\ttype\ttarget"
    #define REJECTED_COLUMNS_1 "video_id\tduration_s\ttimestamp\ttitle"
    #define REJECTED_COLUMNS_2 "\tin_range\tarchived"
    #define REJECTED_COLUMNS REJECTED_COLUMNS_1 REJECTED_COLUMNS_2
    #define SOURCES_COLUMNS "playlist_id\turl\tmin_s\tmax_s\treport_absent"
    #define ENVELOPES_COLUMNS "hash\tenvelope"
    #define NOT_DUPS_COLUMNS "song_a\tsong_b"
    #define TIES_COLUMNS "board\tkind\tid\tposition"
    #define TSV_MAX_FIELDS 20
    #define V1_FIELDS 14
    #define V2_FIELDS 16
    #define FPRINT_BYTES 262144
    #define DUP_MAX_DIFF_MS 15000
    #define ENV_SHIFT 160
    #define ENV_MIN 100
    #define ENV_MAX_FILE_MS 1200000
    #define DUP_SURE 70
    #define DUP_DIFFERENT 40
    #define DUP_AUTO 99

    #define LIST_X 130
    #define LIST_Y 258
    #define SORT_Y 190
    #define TABS_Y 124
    #define TABS_RIGHT 1530
    #define LIST_STEP 64
    #define LIST_THUMB 56
    #define LIST_TEXT_X (LIST_X + LIST_THUMB + 18)
    #define LIST_LIKES_X 1556
    #define LIST_RIGHT 1830
    #define FIT_LEN 1024
    #define BAR_TEXT_W 570
    #define PANEL_H 790
    #define BOARD_BAR_Y 190
    #define CAT_BAR_Y 260
    #define CONTENT_Y 350
    #define BATTLE_CARD_Y 340
    #define BATTLE_CARD_H 450
    #define FIELD_SHOWN 400
    #define STATS_Y 330
    #define CONTENT_BOTTOM 820
    #define BOARD_STEP 56
    #define TILES_X 250
    #define TILES_RIGHT 1700
    #define TILE_MAX 96
    #define TILE_MIN 40
    #define TILE_GAP 8
    #define THUMB 48
    #define PL_ROWS_Y 330
    #define PL_ROW_STEP 60
    #define BOARDS_ROWS_Y 330
    #define DUP_ROWS_Y 250
    #define DUP_HEAD_H 44
    #define DUP_ROW_H 76
    #define DUP_TEXT_W 610
    #define DUP_SAME_H 56
    #define DLPOPUP_ROWS_Y 290
    #define DLPOPUP_STEP 54

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
int key_repeated(app_t *app, sfEvent const *event);
int window_create(app_t *app);
void window_apply_toggle(app_t *app);
int view_create(app_t *app);
void view_update(app_t *app);
void view_mouse(app_t *app);
int signals_block(void);
int signals_pending(void);
int signals_unblock(void);

// src/scene
int scene_switch(app_t *app);
void scene_open(app_t *app, scene_id_t id);
void scene_back(app_t *app);

// src/scene/home
int settings_init(app_t *app);
void settings_event(app_t *app, sfEvent *event);
void settings_draw(app_t *app);
void settings_layout(app_t *app);
void settings_destroy(app_t *app);
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
void list_play(app_t *app, int row);
void list_scroll(app_t *app, float delta);
void list_likes(app_t *app);
void list_draw(app_t *app);
int bar_create(app_t *app);
int bar_seek(app_t *app);
void bar_update(app_t *app);
void bar_draw(app_t *app);
void bar_layout(app_t *app);
void list_free(app_t *app);
void library_likes(app_t *app);
void library_tabs(app_t *app);
void library_sort_bar(app_t *app);
int select_has(selection_t const *sel, int id);
song_t *select_song(app_t *app, int i);
int select_row(app_t *app);
void select_click(app_t *app, int row);
void select_all(app_t *app, int all);
void library_select_toggle(app_t *app);
void library_select_boxes(app_t *app);
void library_select_bar(app_t *app);
void library_actions(app_t *app);
void library_insert_rows(app_t *app, float y);
void library_image_row(app_t *app, float y);
void library_actions_key(app_t *app, sfUint32 code);
void insert_title(selection_t const *sel, char const *title, char *out);
void bulk_insert(app_t *app);
void bulk_suggest(app_t *app);
void bulk_like(app_t *app, int value);
void bulk_playlist(app_t *app, int add);
void bulk_images(app_t *app, int from_cover);
void bulk_remove(app_t *app, int exclude);
void library_reorder(app_t *app, int keep_scroll);
void list_values(app_t *app);
void drag_press(app_t *app);
void drag_update(app_t *app);
int drag_release(app_t *app);
void drag_draw(app_t *app);

// src/scene/edit
int edit_init(app_t *app);
void edit_event(app_t *app, sfEvent *event);
void edit_draw(app_t *app);
void edit_layout(app_t *app);
void edit_destroy(app_t *app);
void edit_remove_buttons(app_t *app, edit_t *edit, float y);
void edit_main(app_t *app);
void edit_search(app_t *app);
void edit_confirm(app_t *app);
void edit_artist(app_t *app);
void edit_artist_row(app_t *app, edit_t *edit, int i);
void edit_text_entered(app_t *app, sfUint32 code);
void edit_add_artist(app_t *app, char const *name, role_t role);
void edit_refresh_lists(app_t *app);
void edit_suggest_line(app_t *app, edit_t *edit);
void edit_titles(app_t *app);

// src/scene/battle
int battle_init(app_t *app);
void battle_event(app_t *app, sfEvent *event);
void battle_draw(app_t *app);
void battle_layout(app_t *app);
void battle_destroy(app_t *app);
void battle_view(app_t *app);
void battle_next(app_t *app);
void battle_vote(app_t *app, int side);
int battle_listen(app_t *app, rank_t const *rank, float x);
void battle_frame(app_t *app, float x);
void battle_reload(app_t *app);
void battle_restore(app_t *app);

// src/scene/ranking
int ranking_init(app_t *app);
void ranking_event(app_t *app, sfEvent *event);
void ranking_draw(app_t *app);
void ranking_layout(app_t *app);
void ranking_destroy(app_t *app);
void ranking_build(app_t *app);
void ranking_view(app_t *app);
void ranking_tiers(app_t *app);
int ranking_tier(int rank, int nb);
int ranking_tiles(app_t *app, int tier, float const *row);
void ranking_board(app_t *app);
void ranking_scroll(app_t *app, float delta);
int ranking_match(app_t *app, rank_t const *rank);
void ranking_hover(app_t *app, rank_t const *rank, float const *rect);
void ranking_open_hover(app_t *app);
void ranking_restore(app_t *app);
int ranking_count(app_t *app);
void ranking_search(app_t *app);
int ranking_rows(app_t *app);
void ranking_ties(app_t *app);
void ranking_drag_press(app_t *app);
int ranking_drag_release(app_t *app);
void ranking_drag_draw(app_t *app);
void ranking_reset_view(app_t *app);
void ranking_assign_tiers(ranking_t *ranking);
void ranking_tie_keys(app_t *app, ranked_t *items, int nb);
void ranking_save_order(app_t *app);

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

// src/scene/download
int dlscene_init(app_t *app);
void dlscene_event(app_t *app, sfEvent *event);
void dlscene_draw(app_t *app);
void dlscene_layout(app_t *app);
void dlscene_destroy(app_t *app);
void dlscene_start(app_t *app);
void dlscene_view(app_t *app);
void dlscene_stats(app_t *app, download_t const *dl);
void dlscene_status(app_t *app, download_t const *dl);
void dlscene_failures(app_t *app, download_t const *dl, float y);
void download_order_free(download_t *dl);
void download_keep_order(app_t *app);
void download_apply_order(app_t *app);
void download_order_swap(download_t *dl, char const *old_id,
    library_t *lib, song_t *song);
song_t *reupload_match(library_t *lib, dl_entry_t const *entry);
int download_reupload(app_t *app, dl_entry_t const *entry);
void dlscene_options(app_t *app, dl_options_t *opt);
void dlscene_text(app_t *app, sfUint32 code);
void dlscene_durations(app_t *app, float y);
int parse_duration(char const *text);
void duration_text(char *buf, int secs);
void dlscene_range_load(app_t *app);
void dlscene_range_save(app_t *app);
void dlscene_preselect(app_t *app);
void dlscene_targets(app_t *app, float y);
void dlscene_popup(app_t *app);
void dlscene_sync(app_t *app);
void dlscene_link(app_t *app, float y);
void dlscene_absent(app_t *app);
void dlscene_absent_footer(app_t *app, float y);
void dlscene_report(app_t *app);
int dlscene_report_lines(app_t *app);
void dlscene_excluded(app_t *app);
void dlscene_refused(app_t *app);
int dlpopup_shown(app_t *app, rejected_t const *item, int *i);
void dlscene_archive_footer(app_t *app, float y);
int dlscene_split_count(app_t *app);
void dlscene_splits(app_t *app);
void roles_explode(artist_role_t *roles, int *nb, int i);
int dlpopup_rows(app_t *app);
void format_size(char *buf, size_t size, double bytes);

// src/scene/duplicates
int dupscene_init(app_t *app);
void dupscene_event(app_t *app, sfEvent *event);
void dupscene_update(app_t *app);
void dupscene_draw(app_t *app);
void dupscene_layout(app_t *app);
void dupscene_destroy(app_t *app);
int dupscene_todo(app_t *app);
void dupscene_clean(app_t *app);
void dupscene_scroll(app_t *app, float delta);
void dupscene_view(app_t *app);
int dup_block(app_t *app, dup_group_t *group, float y);
float dup_block_h(dup_group_t const *group);
int dup_row(app_t *app, dup_group_t const *group, int i, float y);
void dup_keep(app_t *app, song_t *keep, song_t *other);
void dup_keep_all(app_t *app, dup_group_t *group, int i);
int dup_auto_merge(app_t *app);
int dup_same_count(app_t *app);
same_file_t *dup_same_get(app_t *app, int n);
int dup_same_row(app_t *app, int n, float y);

// src/download : audio download by yt-dlp
void report_add(download_t *dl, report_status_t status, char const *title,
    char const *detail);
void report_free(download_t *dl);
int report_count(download_t const *dl, report_status_t status);
void report_finish(download_t *dl, int stopped);
int download_start(app_t *app, char const *text, dl_options_t const *opt);
int download_begin(app_t *app, char **queue, dl_options_t const *opt);
int download_launch(app_t *app, char const *url, dl_step_t step);
int program_found(char const *name);
void swap_exec(download_t const *dl, char const *url);
void download_list_done(app_t *app, int success);
void download_swap_done(app_t *app, int success);
char const *swap_detail(dl_entry_t const *entry);
int swap_read(app_t *app);
int download_next(app_t *app);
void download_cancel(app_t *app);
void download_poll(app_t *app);
void download_archive_write(library_t const *lib);
void download_sort(app_t *app);
void download_added(app_t *app, dl_entry_t const *entry);
int download_list_line(download_t *dl, char const *line);
void entries_free(dl_entry_t *list);
void entry_append(dl_entry_t **list, dl_entry_t *entry);
dl_entry_t *entry_find(dl_entry_t *list, char const *id);
dl_entry_t *entry_pop(dl_entry_t **list);
int entries_count(dl_entry_t const *list);
void download_accept(app_t *app, dl_entry_t *entry);
void download_refuse(app_t *app, dl_entry_t *entry);
void download_exclude(app_t *app, dl_entry_t *entry);
void download_decide_all(app_t *app, int accept);
void download_find_absent(app_t *app);
void download_absent_keep(app_t *app, int i);
void download_absent_unlist(app_t *app, int i);
void download_absent_delete(app_t *app, int i);
void stats_parse(dl_stats_t *st, char const *line);
void stats_item_done(dl_stats_t *st);
int download_rest(download_t const *dl);
long download_estimate(download_t const *dl, double *size);

// src/scene/pages : pieces of screens usable by several scenes
void artist_page(app_t *app, artist_t *artist, int *back);
int like_row(app_t *app, song_t const *song, float const *rect, float x);
int like_buttons(app_t *app, elo_kind_t kind, int id, float const *pos);
int category_bar(app_t *app, int *category, int nb, int random);
int board_bar(app_t *app, arena_t *arena);
int image_box(app_t *app, rank_t const *rank, image_input_t *input,
    float const *pos);
int listen_mine(app_t *app, song_t const *song);
int listen_button(app_t *app, song_t *song, float const *rect);
void listen_time(app_t *app, float const *pos);
int listen_bar(app_t *app, float const *bar);
void search_field(app_t *app, float const *rect);
int search_event(app_t *app, sfEvent const *event);
int search_text(app_t *app, char const *text);
int search_song(app_t *app, song_t const *song);

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
int widget_field(app_t *app, float const *rect, char *value,
    int focused);
void widget_box(app_t *app, float const *rect, int focused);
void widget_place(app_t *app, float const *rect);
void widget_rect(app_t *app, float const *rect, sfColor const *color);
float widget_text_width(app_t *app, char const *str, unsigned int size);
void widget_image(app_t *app, sfTexture *texture, float const *rect,
    int round);
void widget_tip(app_t *app, float const *rect, char const *str);
void widget_tip_draw(app_t *app);
void widget_help(app_t *app, float const *rect, char const *str);
int widget_icon(app_t *app, float const *rect, int icon, int state);
void text_fit(app_t *app, sfText *text, char const *str, float max_w);
void widget_fit(app_t *app, char const *str, float const *pos,
    sfColor const *color);
void widget_song(app_t *app, song_t const *song, float const *rect,
    sfColor const *color);
void widget_rank(app_t *app, rank_t const *rank, float const *rect,
    sfColor const *color);
int widget_icon_button(app_t *app, float const *rect, int icon,
    char const *label);
sfTexture *image_get(app_t *app, rank_t const *rank, int small);
void image_cache_clear(app_t *app);

// src/audio
void player_play(app_t *app, int index);
void player_toggle(app_t *app);
void player_skip(app_t *app, int direction);
void player_stop(app_t *app, end_reason_t reason);
void player_update(app_t *app);
void player_resync(app_t *app);
void player_shuffle_toggle(app_t *app);
void player_repeat_next(app_t *app);
void player_seek(app_t *app, long delta);
void player_halt(app_t *app);
void player_volume_apply(app_t *app);
void player_volume_step(app_t *app, int step);
void player_mute_toggle(app_t *app);
void queue_shuffle(song_t **queue, int nb, song_t const *first);
int queue_keep_order(song_t **fresh, int nb, player_t const *old);
void library_play_modes(app_t *app);
int stats_init(app_t *app);
void stats_event(app_t *app, sfEvent *event);
void stats_draw(app_t *app);
void stats_layout(app_t *app);
void stats_destroy(app_t *app);
void stats_build(app_t *app);
void stats_bar(app_t *app);
void stats_dates(app_t *app);
void stats_view(app_t *app);
void stats_scroll(app_t *app, float delta);
int stats_rows_shown(app_t *app);
int stats_log_load(listen_t **out);
void stat_totals(library_t *lib, stat_row_t *row);
void stat_period(library_t *lib, statscene_t const *scene,
    long const *range, stat_row_t *by_id);
long stat_date(char const *text, int end);
void stat_sort(stat_row_t *rows, int nb, int sort, int ascending);
void player_queue(app_t *app, int playlist_id, char const *filter);
void preview_stop(app_t *app);
void preview_toggle(app_t *app, song_t *song);
void preview_seek(app_t *app, float ratio);
void preview_update(app_t *app);
void listen_log(song_t const *song, long ms, end_reason_t reason);

// src/library
library_t *library_load(void);
int library_refresh(library_t *lib);
void library_destroy(library_t *lib);
void library_rescan(app_t *app);
void library_drop_missing(app_t *app);

// src/library/file : files on disk
long id3_synchsafe(unsigned char const *bytes);
long mp3_audio_offset(unsigned char const *head);
int mp3_is_valid(char const *path);
char *fingerprint_file(char const *path);
void library_fill_durations(library_t *lib);
song_t *scan_music(char const *dir);
song_t *scan_file(char const *dir, char const *name);

// src/library/id3 : tags
void id3_read(song_t *song, char const *path);
void id3_parse_frames(song_t *song, unsigned char const *tag, long size,
    int version);
char *id3_decode_text(unsigned char const *data, long size);
long id3_frame_size(unsigned char const *b, int version);
int id3_load(char const *path, id3_tag_t *tag);
unsigned char *id3_cover(char const *path, long *size);

// src/library/images : images of the songs and artists
void image_path(char *buf, rank_t const *rank, int small);
int image_import(rank_t const *rank, char const *file);
int image_from_cover(rank_t const *rank);
void images_from_covers(library_t *lib);
void covers_repair(library_t *lib);
int thumbnail_save(sfImage *source, char const *big, char const *small);
void path_clean(char const *in, char *out);

// src/library/songs
song_t *song_create(void);
void song_destroy(song_t *song);
int song_forgettable(library_t *lib, song_t const *song);
int song_forget(app_t *app, song_t *song);
void song_forget_refs(library_t *lib, int id);
void song_forget_logs(song_t *song);
artist_t *artist_rename(app_t *app, artist_t *artist, char const *name);
void edit_rename_artist(app_t *app);
int artist_split_parts(char const *name, char (*parts)[NAME_LEN]);
int artist_split_doubtful(char (*parts)[NAME_LEN], int nb);
void artist_split_apply(library_t *lib, artist_t *artist);
int artist_split_state(library_t *lib, artist_t const *artist);
void artists_split_auto(library_t *lib);
split_choice_t *split_choice_find(library_t *lib, char const *name);
void split_choice_set(library_t *lib, char const *name, int keep);
void split_choices_free(library_t *lib);
void db_parse_split(library_t *lib, char **fields, int nb);
int db_save_splits(library_t *lib);
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
void song_delete_files(app_t *app, song_t *song);
void song_remove_files(library_t *lib, song_t *song);
void song_log_delete(int song_id, char const *path);
int merge_warnings(song_t const *src, song_t const *dst,
    char warn[EDIT_WARNINGS][128]);

// src/library/file/same_files.c : files with the sound of another one
void same_file_add(library_t *lib, song_t *found, int song_id);
void same_files_free(library_t *lib);
song_t *same_file_video(library_t *lib, char const *id);
void same_file_delete(same_file_t *same);
void same_files_of(library_t *lib, song_t *song);

// src/library/videos : videos the songs come from
int source_video_id(char const *source, char *id);
int video_in_name(char const *path, char const *id);
int song_video_id(library_t *lib, song_t *song, char *id);
rejected_t *excluded_find(library_t *lib, char const *id);
rejected_t *excluded_add(library_t *lib, char const *id, char const *title);
void excluded_remove(library_t *lib, char const *id);
int song_exclude(app_t *app, song_t *song);
int excluded_count(library_t const *lib, int archived);
rejected_t *unavailable_find(library_t *lib, char const *id);
rejected_t *unavailable_add(library_t *lib, char const *id,
    char const *title, char const *reason);
void unavailable_remove(library_t *lib, char const *id);
int unavailable_count(library_t const *lib);
void dl_reason(char *out, size_t size, char const *detail);
int dl_reason_lasting(char const *detail);
void dlscene_unavailable(app_t *app);
void dlscene_unavailable_footer(app_t *app, float y);
song_t *song_by_video(library_t *lib, char const *id);
void excluded_songs(library_t *lib);
rejected_t *rejected_find(library_t *lib, char const *id);
rejected_t *rejected_add(library_t *lib, char const *id, char const *title,
    int duration);
void rejected_remove(library_t *lib, char const *id);
int rejected_count(library_t const *lib, int archived);
source_t *source_get(library_t *lib, int playlist_id);
source_t *source_set(library_t *lib, int playlist_id, char const *url);
void source_remove(library_t *lib, int playlist_id);
int fprint_video_id(fprint_t const *fp, char *id);

// src/library/duplicates : songs present twice
void dup_clean(char const *in, char *out, int size);
int dup_has_word(char const *clean, char const *word);
int dup_version(char const *clean);
void dup_remove_noise(char *clean);
void dup_core(char const *title, char *core, char *prefix);
void dup_artists(library_t *lib, song_t const *song, char const *prefix,
    char *out);
int dup_artists_meet(char const *a, char const *b);
void dup_names(char *out, char const *names);
int dup_find(library_t *lib, dup_list_t *list);
int dup_group_pairs(dup_list_t *list);
void dup_group_remove(dup_group_t *group, int i);
unsigned char *envelope_compute(char const *path, int *nb);
int envelope_compare(envelope_t const *a, envelope_t const *b);
envelope_t *envelope_find(library_t *lib, char const *hash);
envelope_t *envelope_add(library_t *lib, char const *hash,
    unsigned char *data, int nb);
void envelopes_free(library_t *lib);
char const *song_hash(library_t *lib, song_t const *song);
envelope_t *song_envelope(library_t *lib, song_t const *song);
int not_dup_find(library_t *lib, song_t const *a, song_t const *b);
int not_dup_add(library_t *lib, int a, int b);
void not_dups_free(library_t *lib);

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
int playlist_append(library_t *lib, playlist_t *playlist, song_t *song);
int playlists_append(library_t *lib, int const *ids, int nb, song_t *song);
sort_def_t const *sort_def(int key);
int sort_key_by_name(char const *name);
view_sort_t *sort_get(library_t *lib, int playlist_id);
void sort_set(library_t *lib, int playlist_id, sort_key_t key);
void sort_remove(library_t *lib, int playlist_id);
int song_matches(song_t const *song, char const *text);
int view_filter(song_t **view, int nb, char const *text);
int playlist_sorted_view(library_t *lib, playlist_t const *playlist,
    song_t **view);
int sort_set_custom(library_t *lib, view_sort_t *sort, song_t *const *view,
    int nb);
long custom_position(view_sort_t const *sort, song_t *const *custom,
    song_t const *song, int native);
int playlist_view(library_t *lib, playlist_t const *playlist,
    song_t **view);

// src/library/boards : rankings and their rules
board_t *board_add(library_t *lib, int id, char const *name);
board_t *board_by_id(library_t *lib, int id);
void board_seen_id(library_t *lib, int board_id);
void boards_sync_playlists(library_t *lib);
tie_order_t *tie_find(library_t *lib, int board, int kind);
int tie_append(library_t *lib, int board, int kind, int id);
int tie_set(library_t *lib, arena_t const *arena, int const *ids, int nb);
void ties_free(library_t *lib);
int board_general_create(library_t *lib);
void board_remove(library_t *lib, board_t *board);
int rule_add(board_t *board, int include, rule_type_t type, int target);
void rule_remove(board_t *board, rule_t *rule);
void rule_text(library_t *lib, rule_t const *rule, char *buf, int size);
char const *rule_type_name(rule_type_t type);
void rules_forget(library_t *lib, rule_type_t type, int target);
int board_has_song(library_t *lib, int board_id, song_t *song);

// src/library/likes : likes and the automatic playlists
int like_get(library_t *lib, elo_kind_t kind, int id);
int like_add(library_t *lib, elo_kind_t kind, int id, int value);
void like_set(library_t *lib, elo_kind_t kind, int id, int value);
void like_put(library_t *lib, elo_kind_t kind, int id, int value);
void likes_free(library_t *lib);
playlist_t *playlist_auto(library_t *lib, int value);
void likes_sync(library_t *lib);

// src/library/suggest : suggestions to clean a title
char const *suggest_rule_name(int rule);
int suggest_rule_find(char const *name);
void suggest_list(library_t *lib, song_t *song, char const *title,
    suggest_list_t *out);
int sug_noise_word(char const *word);
int sug_style_name(char const *name);
int sug_license_at(char const *text);
int sug_song_name(suggest_ctx_t const *ctx, char const *name, int any_known);
void sug_tidy(char *title);
void sug_cut(char const *title, int start, int end, char *out);
suggest_t *sug_add(suggest_ctx_t *ctx, suggest_rule_t rule,
    char const *removed, char const *title);
suggest_t *sug_push(suggest_ctx_t *ctx, suggest_rule_t rule, int start,
    int len);
void sug_prefix(suggest_ctx_t *ctx);
void sug_guess(suggest_ctx_t *ctx);
int suggest_is_guess(int rule);
void sug_credit(suggest_ctx_t *ctx, int const *where, char const *name,
    int role);
void sug_suffix(suggest_ctx_t *ctx);
void sug_hashtags(suggest_ctx_t *ctx);
void sug_prod(suggest_ctx_t *ctx);
void sug_groups(suggest_ctx_t *ctx);
void sug_remix(suggest_ctx_t *ctx, int const *group, char const *name,
    int len);
void sug_bare_feat(suggest_ctx_t *ctx);
int suggest_state(library_t *lib, int song, suggest_t const *sug);
int suggest_state_add(library_t *lib, int song, suggest_t const *sug,
    int state);
void suggest_set_state(library_t *lib, int song, suggest_t const *sug,
    int state);
void suggest_states_free(library_t *lib);
void suggest_split_names(artist_role_t *roles, int *nb, char const *artist);
void suggest_apply_roles(artist_role_t *roles, int *nb, suggest_t const *sug);
int song_roles(library_t *lib, song_t const *song, artist_role_t *out);
int suggest_first(library_t *lib, song_t *song, char const *title,
    suggest_list_t *list);
int suggest_apply_song(library_t *lib, song_t *song);

// src/elo : ratings, without any display
int category_find(char const *name);
char const *category_name(int category);
int category_rated(int category);
int kind_find(char const *name);
char const *kind_name(elo_kind_t kind);
elo_t *elo_find(library_t *lib, arena_t const *arena, int subject);
elo_t *elo_get(library_t *lib, arena_t const *arena, int subject);
void elo_battle(library_t *lib, arena_t const *arena, int winner,
    int loser);
void elo_clear(library_t *lib);
int elo_parse_reset(library_t *lib, char **fields, int nb);
void elo_reset(library_t *lib, arena_t const *arena, int all);
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
void db_parse_sort(library_t *lib, char **fields, int nb);
void db_parse_custom(library_t *lib, char **fields, int nb);
int db_save_sorts(library_t *lib);
void db_parse_board(library_t *lib, char **fields, int nb);
void db_parse_rule(library_t *lib, char **fields, int nb);
int db_save_boards(library_t *lib);
void db_parse_rejected(library_t *lib, char **fields, int nb);
int db_save_rejected(library_t *lib);
void db_parse_source(library_t *lib, char **fields, int nb);
int db_save_sources(library_t *lib);
void db_parse_envelope(library_t *lib, char **fields, int nb);
int db_save_envelopes(library_t *lib);
void db_parse_not_dup(library_t *lib, char **fields, int nb);
int db_save_not_dups(library_t *lib);
void log_line(char const *path, char const *columns, char const *line);
void db_load_elo(library_t *lib);
void db_parse_tie(library_t *lib, char **fields, int nb);
int db_save_ties(library_t *lib);
void db_parse_like(library_t *lib, char **fields, int nb);
int db_save_likes(library_t *lib);
void db_parse_excluded(library_t *lib, char **fields, int nb);
int db_save_excluded(library_t *lib);
void db_parse_unavailable(library_t *lib, char **fields, int nb);
int db_save_unavailable(library_t *lib);
void db_parse_suggestion(library_t *lib, char **fields, int nb);
void db_parse_setting(library_t *lib, char **fields, int nb);
void db_load_settings(library_t *lib);
int db_save_settings(library_t *lib);
int db_save_suggestions(library_t *lib);
int db_save_elo(library_t *lib);
void battle_record(library_t *lib, arena_t const *arena,
    rank_t const *winner, rank_t const *loser);

// src/utils
int utf8_encode(char *out, unsigned int code);
sfUint32 *utf8_to_utf32(char const *str);
void utf8_cut(char *buf, char const *text, size_t max);
char *str_lower(char const *str);
char *str_or_null(char const *str);
int str_contains(char const *hay, char const *needle);
int split_tab(char *line, char **fields, int max);
void str_trim(char *str);
int text_input(app_t *app, char *buf, sfUint32 code);
int text_input_n(app_t *app, char *buf, size_t size, sfUint32 code);
size_t utf8_prev(char const *str, size_t pos);
size_t utf8_next(char const *str, size_t pos);
int utf8_count(char const *str, size_t bytes);
caret_t *caret_of(app_t *app, char *buf);
int caret_insert(app_t *app, char *buf, size_t size, sfUint32 code);
int caret_erase(app_t *app, char *buf, int forward);
int caret_key(app_t *app, sfEvent const *event);
char **stwa_delimiter(char const *str, char const *delim, int didx,
    int empty);
void str_array_free(char **arr);
void format_time(char *buf, size_t size, long ms);

#endif
