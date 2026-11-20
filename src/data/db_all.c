/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** loads and saves the whole database
*/

#include "mp3player.h"

/*
** the playlists are read before the rankings, whose rules use them
*/
void db_load_all(library_t *lib)
{
    tsv_read(ARTISTS_DB, &db_parse_artist, lib);
    tsv_read(SONGS_DB, &db_parse_song, lib);
    tsv_read(FPRINTS_DB, &db_parse_fprint, lib);
    tsv_read(LINKS_DB, &db_parse_link, lib);
    tsv_read(PLAYLISTS_DB, &db_parse_playlist, lib);
    tsv_read(PLAYLIST_SONGS_DB, &db_parse_playlist_song, lib);
    tsv_read(BOARDS_DB, &db_parse_board, lib);
    tsv_read(RULES_DB, &db_parse_rule, lib);
    if (lib->migrated)
        db_migrate(lib);
    db_migrate_battles();
}

int db_save_all(library_t *lib)
{
    int ret = db_save_songs(lib);

    if (db_save_tables(lib) == 84 || db_save_elo(lib) == 84)
        ret = 84;
    if (db_save_playlists(lib) == 84 || db_save_boards(lib) == 84)
        ret = 84;
    return ret;
}
