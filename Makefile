##
## EPITECH PROJECT, 2025
## wolf3d
## File description:
## wolf3d general makefile
##

SRC	=	main.c\
		src/isfreeable.c\
		src/mouse_on_sprite.c\
		src/remove_mp3_tag.c\
		src/init/init_music.c\
		src/update/handle_window.c\
		src/update/handle_events.c\
		src/update/handle_textures.c\
		src/update/update_music.c\
                src/init/init_all.c\
		src/init/init_menu.c\
                src/init/init_textures.c\
                src/init/init_playlist.c\
                src/init/init_window.c\
                src/destroy/destroy_all.c\
                src/destroy/destroy_textures.c\
		src/destroy/destroy_window.c\
                src/display/draw_all.c\

OBJ	=	$(SRC:.c=.o)

NAME	=	MP3player

CC	=	clang

CFLAGS	+=	-I include/

FLAGS	=	-lcsfml-graphics -lcsfml-window \
		-lcsfml-system -lcsfml-audio -lm

all:	$(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ)
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

.PHONY: all %.o clean fclean debug re
