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
/*
** what was added since the first version of the files
*/
static void load_more(library_t *lib)
{
    tsv_read(REJECTED_DB, &db_parse_rejected, lib);
    tsv_read(SOURCES_DB, &db_parse_source, lib);
    tsv_read(NOT_DUPS_DB, &db_parse_not_dup, lib);
    tsv_read(ENVELOPES_DB, &db_parse_envelope, lib);
    tsv_read(TIES_DB, &db_parse_tie, lib);
    tsv_read(LIKES_DB, &db_parse_like, lib);
    tsv_read(EXCLUDED_DB, &db_parse_excluded, lib);
    tsv_read(SUGGEST_DB, &db_parse_suggestion, lib);
}

void db_load_all(library_t *lib)
{
    db_load_settings(lib);
    tsv_read(ARTISTS_DB, &db_parse_artist, lib);
    tsv_read(SONGS_DB, &db_parse_song, lib);
    tsv_read(FPRINTS_DB, &db_parse_fprint, lib);
    tsv_read(LINKS_DB, &db_parse_link, lib);
    tsv_read(PLAYLISTS_DB, &db_parse_playlist, lib);
    tsv_read(PLAYLIST_SONGS_DB, &db_parse_playlist_song, lib);
    tsv_read(SORTS_DB, &db_parse_sort, lib);
    tsv_read(CUSTOM_DB, &db_parse_custom, lib);
    tsv_read(BOARDS_DB, &db_parse_board, lib);
    tsv_read(RULES_DB, &db_parse_rule, lib);
    load_more(lib);
    likes_sync(lib);
    boards_sync_playlists(lib);
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
    if (db_save_sorts(lib) == 84 || db_save_rejected(lib) == 84)
        ret = 84;
    if (db_save_sources(lib) == 84 || db_save_not_dups(lib) == 84)
        ret = 84;
    if (db_save_envelopes(lib) == 84 || db_save_ties(lib) == 84)
        ret = 84;
    if (db_save_likes(lib) == 84 || db_save_excluded(lib) == 84)
        ret = 84;
    if (db_save_suggestions(lib) == 84)
        ret = 84;
    return ret;
}
