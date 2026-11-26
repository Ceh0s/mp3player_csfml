/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the rankings, and going from the list to the edition of one
** ranking and back
*/

#include "mp3player.h"

/*
** the edited ranking becomes the one of the battle and ranking screens
*/
void boards_open(app_t *app, board_t *board)
{
    boards_t *bd = app->boards;

    bd->mode = BOARDS_EDIT;
    bd->board = board;
    bd->focus = 0;
    bd->search[0] = '\0';
    bd->nb_results = 0;
    scroll_jump(&bd->list, 0);
    snprintf(bd->name, NAME_LEN, "%s", board->name);
    app->arena.board = board->id;
    boards_count(app);
}

/*
** back to the list; the typed name is applied if not empty
*/
void boards_leave(app_t *app)
{
    boards_t *bd = app->boards;
    char *name = NULL;

    if (bd->mode == BOARDS_EDIT && bd->board->id != BOARD_GENERAL
        && bd->name[0] != '\0')
        name = strdup(bd->name);
    if (name) {
        free(bd->board->name);
        bd->board->name = name;
    }
    bd->mode = BOARDS_LIST;
    bd->board = NULL;
    scroll_jump(&bd->list, 0);
}

/*
** a new ranking starts with every song, to exclude some of them
*/
static void create(app_t *app)
{
    board_t *board = board_add(app->lib, app->lib->next_board,
        "Nouveau classement");

    if (!board)
        return;
    rule_add(board, 1, RULE_ALL, 0);
    boards_open(app, board);
}

/*
** one button per ranking; they scroll
*/
static void rows(app_t *app)
{
    scroll_t *list = &app->boards->list;
    float area[4] = {120, BOARDS_ROWS_Y, 1120,
        CONTENT_BOTTOM + app->extra_h - BOARDS_ROWS_Y};
    int nb = 0;
    float y = 0;

    for (board_t *board = app->lib->boards; board; board = board->next)
        nb += board->id >= 0;
    scroll_frame(list, area, nb * 62);
    scroll_bar_draw(app, list);
    if (!scroll_rows_begin(app, list))
        return;
    nb = 0;
    for (board_t *board = app->lib->boards; board; board = board->next) {
        if (board->id < 0)
            continue;
        y = scroll_y(list, nb * 62);
        nb++;
        if (scroll_seen(list, y, 52) && widget_button(app,
                (float [4]){130, y, 1100, 52}, board->name)) {
            boards_open(app, board);
            break;
        }
    }
    scroll_rows_end(app);
}

void boards_list(app_t *app)
{
    widget_text(app, "Classements", (float [3]){130, 115, 40}, &sfWhite);
    rows(app);
    if (widget_icon_button(app, (float [4]){1300, BOARDS_ROWS_Y, 480, 56},
            ICON_PLUS, "Nouveau classement"))
        create(app);
    if (widget_back(app))
        scene_back(app);
}
