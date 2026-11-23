/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes playlists.tsv and playlist_songs.tsv
** (the songs of a playlist are written in its order)
*/

#include "mp3player.h"

/*
** fields: id, name, auto ("like", "dislike" or nothing; absent in the
** files written before it)
*/
void db_parse_playlist(library_t *lib, char **fields, int nb)
{
    playlist_t *pl = NULL;

    if (nb < 2 || fields[1][0] == '\0')
        return;
    pl = playlist_add(lib, atoi(fields[0]), fields[1]);
    if (!pl || nb < 3)
        return;
    if (strcmp(fields[2], "like") == 0)
        pl->auto_like = 1;
    if (strcmp(fields[2], "dislike") == 0)
        pl->auto_like = -1;
}

/*
** fields: playlist_id, song_id, position (the lines are in order)
*/
void db_parse_playlist_song(library_t *lib, char **fields, int nb)
{
    playlist_t *playlist = NULL;
    int *bigger = NULL;

    if (nb < 2)
        return;
    playlist = playlist_by_id(lib, atoi(fields[0]));
    if (!playlist || atoi(fields[1]) <= 0)
        return;
    bigger = realloc(playlist->songs, sizeof(int) * (playlist->nb + 1));
    if (!bigger)
        return;
    playlist->songs = bigger;
    playlist->songs[playlist->nb] = atoi(fields[1]);
    playlist->nb++;
}

static int save_songs(library_t *lib)
{
    FILE *file = tsv_create(PLAYLIST_SONGS_DB, PLAYLIST_SONGS_COLUMNS);

    if (!file)
        return 84;
    for (playlist_t *pl = lib->playlists; pl; pl = pl->next)
        for (int i = 0; i < pl->nb; i++)
            fprintf(file, "%d\t%d\t%d\n", pl->id, pl->songs[i], i + 1);
    return tsv_commit(file, PLAYLIST_SONGS_DB);
}

static char const *auto_name(int auto_like)
{
    if (auto_like > 0)
        return "like";
    return auto_like < 0 ? "dislike" : "";
}

int db_save_playlists(library_t *lib)
{
    FILE *file = tsv_create(PLAYLISTS_DB, PLAYLISTS_COLUMNS);

    if (!file)
        return 84;
    for (playlist_t *pl = lib->playlists; pl; pl = pl->next)
        fprintf(file, "%d\t%s\t%s\n", pl->id, pl->name,
            auto_name(pl->auto_like));
    if (tsv_commit(file, PLAYLISTS_DB) == 84)
        return 84;
    return save_songs(lib);
}
