##
## EPITECH PROJECT, 2025
## mp3player
## File description:
## mp3player general makefile
##

SRC_CORE	=	src/core/app_destroy.c\
		src/core/app_init.c\
		src/core/keys.c\
		src/core/loop.c\
		src/core/signals.c\
		src/core/view.c\
		src/core/window.c

SRC_SCENE	=	src/scene/scene_nav.c\
		src/scene/scene_table.c\
		src/scene/settings/settings_scene.c\
		src/scene/stats/stats_bar.c\
		src/scene/stats/stats_build.c\
		src/scene/stats/stats_dates.c\
		src/scene/stats/stats_scene.c\
		src/scene/stats/stats_view.c\
		src/scene/battle/battle_listen.c\
		src/scene/battle/battle_scene.c\
		src/scene/battle/battle_view.c\
		src/scene/battle/battle_vote.c\
		src/scene/boards/boards_add.c\
		src/scene/boards/boards_edit.c\
		src/scene/boards/boards_list.c\
		src/scene/boards/boards_scene.c\
		src/scene/download/download_absent_view.c\
		src/scene/download/download_archive_view.c\
		src/scene/download/download_excluded_view.c\
		src/scene/download/download_refused_view.c\
		src/scene/download/download_split_view.c\
		src/scene/download/download_link.c\
		src/scene/download/download_options.c\
		src/scene/download/download_popup.c\
		src/scene/download/download_range.c\
		src/scene/download/download_report_view.c\
		src/scene/download/download_scene.c\
		src/scene/download/download_stats_view.c\
		src/scene/download/download_summary.c\
		src/scene/download/download_targets.c\
		src/scene/download/download_unavailable_view.c\
		src/scene/download/download_view.c\
		src/scene/duplicates/dup_analysis.c\
		src/scene/duplicates/dup_refresh.c\
		src/scene/duplicates/dup_merge.c\
		src/scene/duplicates/dup_row.c\
		src/scene/duplicates/dup_same.c\
		src/scene/duplicates/dup_block.c\
		src/scene/duplicates/dup_scene.c\
		src/scene/duplicates/dup_view.c\
		src/scene/edit/edit.c\
		src/scene/edit/edit_artist.c\
		src/scene/edit/edit_input.c\
		src/scene/edit/edit_main.c\
		src/scene/edit/edit_remove.c\
		src/scene/edit/edit_suggest.c\
		src/scene/edit/edit_rows.c\
		src/scene/edit/edit_search.c\
		src/scene/home/home.c\
		src/scene/library/library_bar.c\
		src/scene/library/library_layout.c\
		src/scene/library/library_likes.c\
		src/scene/library/library_modes.c\
		src/scene/library/library_list.c\
		src/scene/library/library_action_rows.c\
		src/scene/library/library_actions.c\
		src/scene/library/library_bulk.c\
		src/scene/library/library_drag.c\
		src/scene/library/library_insert.c\
		src/scene/library/library_scene.c\
		src/scene/library/library_select.c\
		src/scene/library/library_select_view.c\
		src/scene/library/library_sort.c\
		src/scene/library/library_tabs.c\
		src/scene/library/library_view.c\
		src/scene/pages/artist_page.c\
		src/scene/pages/board_bar.c\
		src/scene/pages/category_bar.c\
		src/scene/pages/image_box.c\
		src/scene/pages/like_buttons.c\
		src/scene/pages/page_open.c\
		src/scene/pages/listen_bar.c\
		src/scene/pages/search_field.c\
		src/scene/playlist/pledit_scene.c\
		src/scene/playlist/pledit_view.c\
		src/scene/ranking/ranking_board.c\
		src/scene/ranking/ranking_build.c\
		src/scene/ranking/ranking_drag.c\
		src/scene/ranking/ranking_order.c\
		src/scene/ranking/ranking_open.c\
		src/scene/ranking/ranking_reset.c\
		src/scene/ranking/ranking_scene.c\
		src/scene/ranking/ranking_search.c\
		src/scene/ranking/ranking_tiers.c\
		src/scene/ranking/ranking_ties.c\
		src/scene/ranking/ranking_tiles.c\
		src/scene/ranking/ranking_view.c

SRC_UI	=	src/ui/image_cache.c\
		src/ui/ui_element.c\
		src/ui/ui_hit.c\
		src/ui/ui_shapes.c\
		src/ui/widget_help.c\
		src/ui/widget_image.c\
		src/ui/widget_shapes.c\
		src/ui/widgets.c\
		src/ui/widget_field.c\
		src/ui/widget_icon.c\
		src/ui/widget_song.c\
		src/ui/text_fit.c\
		src/ui/text_fallback.c

SRC_ELO	=	src/elo/elo_categories.c\
		src/elo/elo_pair.c\
		src/elo/elo_pool.c\
		src/elo/elo_rating.c\
		src/elo/elo_reset.c\
		src/elo/elo_score.c

SRC_AUDIO	=	src/audio/listen_tracker.c\
		src/audio/player.c\
		src/audio/player_queue.c\
		src/audio/player_shuffle.c\
		src/audio/preview.c\
		src/audio/player_controls.c

SRC_LIBRARY	=	src/library/library.c\
		src/library/library_destroy.c\
		src/library/library_rescan.c\
		src/library/stats/stat_sort.c\
		src/library/stats/stat_sums.c\
		src/library/stats/stats_log.c\
		src/library/artists/artist_breakdown.c\
		src/library/boards/board_members.c\
		src/library/boards/board_playlists.c\
		src/library/boards/tie_order.c\
		src/library/boards/board_rules.c\
		src/library/boards/boards.c\
		src/library/artists/artists.c\
		src/library/artists/artists_names.c\
		src/library/artists/artists_stats.c\
		src/library/artists/artist_rename.c\
		src/library/artists/artist_split.c\
		src/library/artists/split_choices.c\
		src/library/artists/link_order.c\
		src/library/duplicates/dup_artists.c\
		src/library/duplicates/dup_audio.c\
		src/library/duplicates/dup_clean.c\
		src/library/duplicates/dup_core.c\
		src/library/duplicates/dup_find.c\
		src/library/duplicates/dup_groups.c\
		src/library/duplicates/envelopes.c\
		src/library/duplicates/not_dup.c\
		src/library/file/durations.c\
		src/library/file/fingerprint.c\
		src/library/file/mp3_check.c\
		src/library/file/scan_music.c\
		src/library/file/same_files.c\
		src/library/file/missing_files.c\
		src/library/id3/id3_cover.c\
		src/library/id3/id3_frames.c\
		src/library/id3/id3_read.c\
		src/library/id3/id3_text.c\
		src/library/likes/like_playlists.c\
		src/library/suggest/suggest.c\
		src/library/suggest/suggest_apply.c\
		src/library/suggest/suggest_ends.c\
		src/library/suggest/suggest_feat.c\
		src/library/suggest/suggest_groups.c\
		src/library/suggest/suggest_names.c\
		src/library/suggest/suggest_state.c\
		src/library/suggest/suggest_text.c\
		src/library/suggest/suggest_words.c\
		src/library/likes/likes.c\
		src/library/images/cover_check.c\
		src/library/images/image_path.c\
		src/library/images/images.c\
		src/library/images/thumbnail.c\
		src/library/playlists/custom_order.c\
		src/library/playlists/playlist_append.c\
		src/library/playlists/playlist_sort.c\
		src/library/playlists/playlist_view.c\
		src/library/playlists/view_filter.c\
		src/library/playlists/playlists.c\
		src/library/playlists/sorts.c\
		src/library/songs/fprints.c\
		src/library/songs/song.c\
		src/library/songs/song_delete.c\
		src/library/songs/song_forget.c\
		src/library/songs/song_forget_refs.c\
		src/library/songs/song_forget_logs.c\
		src/library/songs/song_edit.c\
		src/library/songs/song_key.c\
		src/library/songs/song_merge.c\
		src/library/songs/song_resolve.c\
		src/library/videos/excluded.c\
		src/library/videos/excluded_song.c\
		src/library/videos/unavailable.c\
		src/library/videos/rejected.c\
		src/library/videos/sources.c\
		src/library/videos/video_source.c

SRC_DOWNLOAD	=	src/download/dl_reason.c\
		src/download/download_absent.c\
		src/download/download_add.c\
		src/download/download_archive.c\
		src/download/download_flagged.c\
		src/download/download_list.c\
		src/download/download_order.c\
		src/download/download_reupload.c\
		src/download/download_poll.c\
		src/download/download_queue.c\
		src/download/download_report.c\
		src/download/download_sort.c\
		src/download/download_swap.c\
		src/download/swap_parse.c\
		src/download/download_stats.c\
		src/download/download_run.c

SRC_DATA	=	src/data/db_all.c\
		src/data/db_boards.c\
		src/data/db_dups.c\
		src/data/db_elo.c\
		src/data/db_merge.c\
		src/data/db_migrate.c\
		src/data/db_playlists.c\
		src/data/db_rejected.c\
		src/data/db_sources.c\
		src/data/db_sorts.c\
		src/data/db_songs.c\
		src/data/db_settings.c\
		src/data/db_suggest.c\
		src/data/db_tables.c\
		src/data/db_excluded.c\
		src/data/db_unavailable.c\
		src/data/db_likes.c\
		src/data/db_ties.c\
		src/data/logs.c\
		src/data/tsv.c

SRC_UTILS	=	src/utils/str_split.c\
		src/utils/str_utils.c\
		src/utils/text_input.c\
		src/utils/text_caret.c\
		src/utils/utf8_step.c\
		src/utils/utf8.c

SRC	=	main.c\
		$(SRC_CORE)\
		$(SRC_SCENE)\
		$(SRC_UI)\
		$(SRC_AUDIO)\
		$(SRC_DOWNLOAD)\
		$(SRC_ELO)\
		$(SRC_LIBRARY)\
		$(SRC_DATA)\
		$(SRC_UTILS)

OBJ	=	$(SRC:.c=.o)

NAME	=	MP3player

CC	=	clang

CFLAGS	+=	-I include/ -Wall -Wextra

FLAGS	=	-lcsfml-graphics -lcsfml-window \
		-lcsfml-system -lcsfml-audio -lm

all:	$(NAME)

$(NAME):	$(OBJ)
	@$(CC) -o $(NAME) $(OBJ) $(FLAGS)
	@make clean -C ./ -s

%.o:	%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

debug:	CC	=	epiclang
debug:	CFLAGS +=	-g
debug:	re

re:	fclean all

.PHONY: all clean fclean debug re
