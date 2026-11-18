/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** loads and saves the whole database
*/

#include "mp3player.h"

void db_load_all(library_t *lib)
{
    tsv_read(ARTISTS_DB, &db_parse_artist, lib);
    tsv_read(SONGS_DB, &db_parse_song, lib);
    tsv_read(FPRINTS_DB, &db_parse_fprint, lib);
    tsv_read(LINKS_DB, &db_parse_link, lib);
    if (lib->migrated)
        db_migrate(lib);
}

int db_save_all(library_t *lib)
{
    int ret = db_save_songs(lib);

    if (db_save_tables(lib) == 84)
        ret = 84;
    return ret;
}
