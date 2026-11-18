##
## EPITECH PROJECT, 2025
## mp3player
## File description:
## mp3player general makefile
##

SRC_CORE	=	src/core/app_destroy.c\
		src/core/app_init.c\
		src/core/loop.c\
		src/core/signals.c\
		src/core/view.c\
		src/core/window.c

SRC_SCENE	=	src/scene/scene_table.c\
		src/scene/edit/edit.c\
		src/scene/edit/edit_artist.c\
		src/scene/edit/edit_input.c\
		src/scene/edit/edit_main.c\
		src/scene/edit/edit_propose.c\
		src/scene/edit/edit_rows.c\
		src/scene/edit/edit_search.c\
		src/scene/home/home.c\
		src/scene/library/library_bar.c\
		src/scene/library/library_layout.c\
		src/scene/library/library_list.c\
		src/scene/library/library_scene.c\
		src/scene/pages/artist_page.c

SRC_UI	=	src/ui/ui_element.c\
		src/ui/ui_hit.c\
		src/ui/ui_shapes.c\
		src/ui/widgets.c

SRC_AUDIO	=	src/audio/listen_tracker.c\
		src/audio/player.c

SRC_LIBRARY	=	src/library/library.c\
		src/library/library_destroy.c\
		src/library/artists/artist_breakdown.c\
		src/library/artists/artists.c\
		src/library/artists/artists_names.c\
		src/library/artists/artists_stats.c\
		src/library/file/fingerprint.c\
		src/library/file/mp3_check.c\
		src/library/file/scan_music.c\
		src/library/id3/id3_frames.c\
		src/library/id3/id3_read.c\
		src/library/id3/id3_text.c\
		src/library/songs/fprints.c\
		src/library/songs/song.c\
		src/library/songs/song_edit.c\
		src/library/songs/song_key.c\
		src/library/songs/song_merge.c\
		src/library/songs/song_resolve.c

SRC_DATA	=	src/data/db_all.c\
		src/data/db_merge.c\
		src/data/db_migrate.c\
		src/data/db_songs.c\
		src/data/db_tables.c\
		src/data/logs.c\
		src/data/tsv.c

SRC_UTILS	=	src/utils/str_utils.c\
		src/utils/utf8.c

SRC	=	main.c\
		$(SRC_CORE)\
		$(SRC_SCENE)\
		$(SRC_UI)\
		$(SRC_AUDIO)\
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
