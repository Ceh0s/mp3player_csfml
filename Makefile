##
## EPITECH PROJECT, 2025
## wolf3d
## File description:
## wolf3d general makefile
##

SRC	=	main.c\
		src/isfreeable.c\
		src/mouse_on_sprite.c\
		src/init/init_music.c\
		src/update/handle_window.c\
		src/update/handle_events.c\
		src/update/handle_textures.c\
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

FLAG	=	-g -W -Wall -Wextra

MAKEFLAGS	=	-j$(shell nproc)

CFLAGS	+=	$(FLAG)

all:	$(NAME)

$(NAME):	$(OBJ)
	@gcc -o $(NAME) $(OBJ) -lcsfml-graphics -lcsfml-window \
	-lcsfml-system -lcsfml-audio -lm
	@make clean -C ./ -s

%.o:	%.c
	@gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all
