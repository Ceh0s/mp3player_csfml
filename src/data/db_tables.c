/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes fingerprints.tsv, artists.tsv and song_artists.tsv
*/

#include "mp3player.h"

void db_parse_fprint(library_t *lib, char **fields, int nb)
{
    if (nb != 2 || fields[0][0] == '\0')
        return;
    fprint_add(lib, fields[0], atoi(fields[1]));
}

void db_parse_artist(library_t *lib, char **fields, int nb)
{
    artist_t *artist = NULL;

    if (nb != 2 || atoi(fields[0]) <= 0 || fields[1][0] == '\0')
        return;
    artist = calloc(1, sizeof(artist_t));
    if (!artist)
        return;
    artist->id = atoi(fields[0]);
    artist->name = strdup(fields[1]);
    artist->next = lib->artists;
    lib->artists = artist;
    if (artist->id >= lib->next_artist)
        lib->next_artist = artist->id + 1;
}

void db_parse_link(library_t *lib, char **fields, int nb)
{
    if (nb != 3)
        return;
    link_add(lib, atoi(fields[0]), atoi(fields[1]),
        role_from_name(fields[2]));
}

static int save_fprints(library_t *lib)
{
    FILE *file = tsv_create(FPRINTS_DB, FPRINTS_COLUMNS);

    if (!file)
        return 84;
    for (fprint_t *fp = lib->fprints; fp; fp = fp->next)
        fprintf(file, "%s\t%d\n", fp->hash, fp->song_id);
    return tsv_commit(file, FPRINTS_DB);
}

static int save_artists(library_t *lib)
{
    FILE *file = tsv_create(ARTISTS_DB, ARTISTS_COLUMNS);

    if (!file)
        return 84;
    for (artist_t *artist = lib->artists; artist; artist = artist->next)
        fprintf(file, "%d\t%s\n", artist->id, artist->name);
    return tsv_commit(file, ARTISTS_DB);
}

static int save_links(library_t *lib)
{
    FILE *file = tsv_create(LINKS_DB, LINKS_COLUMNS);

    if (!file)
        return 84;
    for (link_t *link = lib->links; link; link = link->next)
        fprintf(file, "%d\t%d\t%s\n", link->song_id, link->artist_id,
            role_name(link->role));
    return tsv_commit(file, LINKS_DB);
}

int db_save_tables(library_t *lib)
{
    int ret = 0;

    if (save_fprints(lib) == 84)
        ret = 84;
    if (save_artists(lib) == 84)
        ret = 84;
    if (save_links(lib) == 84)
        ret = 84;
    return ret;
}
