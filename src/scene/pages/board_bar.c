/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** choice of the ranking and of what is compared (songs or artists),
** shared by the battle and ranking screens, with the button opening
** the management of the rankings; board_picker = the same choice of the
** ranking in small, on the stats of a page
*/

#include "mp3player.h"

/*
** the first board has no previous one: the last one is returned
*/
static int previous_board(library_t *lib, board_t const *cur)
{
    board_t *board = lib->boards;

    while (board->next && board->next != cur)
        board = board->next;
    return board->id;
}

static int next_board(library_t *lib, board_t const *cur)
{
    if (cur && cur->next)
        return cur->next->id;
    return lib->boards->id;
}

static void kind_buttons(app_t *app, arena_t *arena)
{
    int songs = arena->kind == KIND_SONG;

    if (widget_button(app, (float [4]){650, BOARD_BAR_Y, 230, 56},
            songs ? "> Musiques <" : "Musiques"))
        arena->kind = KIND_SONG;
    if (widget_button(app, (float [4]){900, BOARD_BAR_Y, 230, 56},
            songs ? "Artistes" : "> Artistes <"))
        arena->kind = KIND_ARTIST;
}

/*
** "<" ">" then "Classement : name", text of the given height at (x, y),
** width = room for the name; returns 1 if the ranking changed
*/
int board_picker(app_t *app, float x, float y, float const *size)
{
    board_t *board = board_by_id(app->lib, app->arena.board);
    float side = size[0] + 8;
    char line[NAME_LEN + 16];
    int old = app->arena.board;

    if (widget_button(app, (float [4]){x, y, side, side}, "<"))
        app->arena.board = previous_board(app->lib, board);
    if (widget_button(app, (float [4]){x + side + 6, y, side, side}, ">"))
        app->arena.board = next_board(app->lib, board);
    snprintf(line, sizeof(line), "Classement : %s", board ? board->name : "?");
    widget_fit(app, line, (float [4]){x + side * 2 + 20, y + 4, size[0],
        size[1]}, &sfWhite);
    return app->arena.board != old;
}

/*
** "<  name  >" browses the rankings; returns 1 if the ranking or the
** kind changed
*/
int board_bar(app_t *app, arena_t *arena)
{
    board_t *board = board_by_id(app->lib, arena->board);
    arena_t old = *arena;

    if (widget_button(app, (float [4]){130, BOARD_BAR_Y, 60, 56}, "<"))
        arena->board = previous_board(app->lib, board);
    widget_text(app, board ? board->name : "?",
        (float [3]){215, BOARD_BAR_Y + 6, 32}, &sfWhite);
    if (widget_button(app, (float [4]){560, BOARD_BAR_Y, 60, 56}, ">"))
        arena->board = next_board(app->lib, board);
    kind_buttons(app, arena);
    if (widget_button(app, (float [4]){1430, BOARD_BAR_Y, 350, 56},
            "Gérer les classements"))
        scene_open(app, BOARDS);
    return arena->board != old.board || arena->kind != old.kind;
}
