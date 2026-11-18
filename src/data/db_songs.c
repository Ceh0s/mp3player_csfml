/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes data/songs.tsv
** v2 : id merged_into file title album genre year src_title src_artists
**      duration_ms first_seen last_seen present listen_ms play_count
**      skip_count
** v1 (old, migrated) : key file title artists album genre year
**      duration_ms first_seen last_seen present listen_ms play_count
**      skip_count
*/

#include "mp3player.h"

static void append_song(library_t *lib, song_t *song)
{
    song_t *last = lib->songs;

    if (!last) {
        lib->songs = song;
        return;
    }
    while (last->next)
        last = last->next;
    last->next = song;
}

static void parse_v2(library_t *lib, song_t *song, char **f)
{
    song->id = atoi(f[0]);
    song->merged_into = atoi(f[1]);
    song->file = str_or_null(f[2]);
    song->title = str_or_null(f[3]);
    song->album = str_or_null(f[4]);
    song->genre = str_or_null(f[5]);
    song->year = str_or_null(f[6]);
    song->src_title = str_or_null(f[7]);
    song->src_artists = str_or_null(f[8]);
    song->duration_ms = atol(f[9]);
    song->first_seen = atol(f[10]);
    song->last_seen = atol(f[11]);
    song->listen_ms = atol(f[13]);
    song->play_count = atoi(f[14]);
    song->skip_count = atoi(f[15]);
    if (song->id >= lib->next_song)
        lib->next_song = song->id + 1;
}

/*
** old format: the song gets a new id, and its key is kept in memory to
** recognise its file during the first scan
*/
static void parse_v1(library_t *lib, song_t *song, char **f)
{
    song->id = lib->next_song;
    lib->next_song++;
    song->key = str_or_null(f[0]);
    song->file = str_or_null(f[1]);
    song->title = str_or_null(f[2]);
    song->src_title = str_or_null(f[2]);
    song->src_artists = str_or_null(f[3]);
    song->album = str_or_null(f[4]);
    song->genre = str_or_null(f[5]);
    song->year = str_or_null(f[6]);
    song->duration_ms = atol(f[7]);
    song->first_seen = atol(f[8]);
    song->last_seen = atol(f[9]);
    song->listen_ms = atol(f[11]);
    song->play_count = atoi(f[12]);
    song->skip_count = atoi(f[13]);
    lib->migrated = 1;
}

void db_parse_song(library_t *lib, char **fields, int nb)
{
    song_t *song = NULL;

    if (nb != V1_FIELDS && nb != V2_FIELDS) {
        fprintf(stderr, "Ignored malformed line in %s\n", SONGS_DB);
        return;
    }
    song = song_create();
    if (!song)
        return;
    if (nb == V2_FIELDS)
        parse_v2(lib, song, fields);
    else
        parse_v1(lib, song, fields);
    append_song(lib, song);
    if (nb == V1_FIELDS)
        artists_from_source(lib, song);
}

static char const *field(char const *str)
{
    return str ? str : "";
}

static void save_song(FILE *file, song_t const *s)
{
    fprintf(file, "%d\t%d\t%s\t%s\t", s->id, s->merged_into,
        field(s->file), field(s->title));
    fprintf(file, "%s\t%s\t%s\t", field(s->album), field(s->genre),
        field(s->year));
    fprintf(file, "%s\t%s\t", field(s->src_title), field(s->src_artists));
    fprintf(file, "%ld\t%ld\t%ld\t%d\t", s->duration_ms, s->first_seen,
        s->last_seen, s->present);
    fprintf(file, "%ld\t%d\t%d\n", s->listen_ms, s->play_count,
        s->skip_count);
}

int db_save_songs(library_t *lib)
{
    FILE *file = tsv_create(SONGS_DB, SONGS_COLUMNS);

    if (!file)
        return 84;
    for (song_t *song = lib->songs; song; song = song->next)
        save_song(file, song);
    return tsv_commit(file, SONGS_DB);
}
