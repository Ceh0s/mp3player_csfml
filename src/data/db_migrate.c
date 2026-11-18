/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** one-time conversion of the v1 save files (songs identified by
** "artist|title") to v2 (songs identified by a number)
** the old files are kept as *.v1.bak
*/

#include "mp3player.h"

static int key_to_id(library_t *lib, char const *key)
{
    for (song_t *song = lib->songs; song; song = song->next)
        if (song->key && strcmp(song->key, key) == 0)
            return song->id;
    return 0;
}

/*
** v1 line : timestamp key listen_ms end
** v2 line : timestamp song_id listen_ms end
*/
static void convert_line(library_t *lib, FILE *out, char *line)
{
    char *f[TSV_MAX_FIELDS];

    if (line[0] == '#' || line[0] == '\n')
        return;
    if (split_tab(line, f, TSV_MAX_FIELDS) != 4)
        return;
    fprintf(out, "%s\t%d\t%s\t%s\n", f[0], key_to_id(lib, f[1]), f[2], f[3]);
}

static void migrate_listens(library_t *lib)
{
    FILE *in = fopen(LISTENS_LOG, "r");
    FILE *out = NULL;
    char *line = NULL;
    size_t size = 0;

    if (!in)
        return;
    out = tsv_create(LISTENS_LOG, LISTENS_COLUMNS);
    if (!out) {
        fclose(in);
        return;
    }
    while (getline(&line, &size, in) != -1)
        convert_line(lib, out, line);
    free(line);
    fclose(in);
    rename(LISTENS_LOG, LISTENS_LOG ".v1.bak");
    tsv_commit(out, LISTENS_LOG);
}

void db_migrate(library_t *lib)
{
    fprintf(stderr, "Converting the save files to the new format "
        "(old files kept as *.v1.bak)\n");
    rename(SONGS_DB, SONGS_DB ".v1.bak");
    migrate_listens(lib);
}
