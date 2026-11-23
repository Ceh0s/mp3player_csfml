/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** every playlist is also a ranking (id -playlist_id, holding the songs
** of the playlist), made again from the playlists after each change of
** them and never saved; they come after the general and custom ones
*/

#include "mp3player.h"

static board_t *first_playlist_board(library_t *lib)
{
    for (board_t *board = lib->boards; board; board = board->next)
        if (board->id < 0)
            return board;
    return NULL;
}

/*
** the battles of a removed playlist stay in battles.log: its id must
** never be given again (it would get them back)
*/
void board_seen_id(library_t *lib, int board_id)
{
    if (board_id >= lib->next_board)
        lib->next_board = board_id + 1;
    if (board_id < 0 && -board_id >= lib->next_playlist)
        lib->next_playlist = -board_id + 1;
}

void boards_sync_playlists(library_t *lib)
{
    board_t *board = first_playlist_board(lib);
    char name[NAME_LEN];

    while (board) {
        board_remove(lib, board);
        board = first_playlist_board(lib);
    }
    for (playlist_t *pl = lib->playlists; pl; pl = pl->next) {
        snprintf(name, sizeof(name), "Playlist : %s", pl->name);
        board = board_add(lib, -pl->id, name);
        if (board)
            rule_add(board, 1, RULE_PLAYLIST, pl->id);
    }
}
