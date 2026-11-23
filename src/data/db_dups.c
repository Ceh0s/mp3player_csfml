/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes the files of the duplicates: envelopes.tsv (sound
** envelope of a file, in hexadecimal, empty if it could not be read)
** and not_duplicates.tsv (pairs marked "Pas un doublon")
*/

#include "mp3player.h"

static int hex_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

void db_parse_envelope(library_t *lib, char **fields, int nb)
{
    int len = nb == 2 ? strlen(fields[1]) / 2 : 0;
    unsigned char *data = len > 0 ? malloc(len) : NULL;

    if (nb != 2 || fields[0][0] == '\0' || envelope_find(lib, fields[0])) {
        free(data);
        return;
    }
    for (int i = 0; data && i < len; i++)
        data[i] = hex_value(fields[1][i * 2]) * 16
            + hex_value(fields[1][i * 2 + 1]);
    envelope_add(lib, fields[0], data, len);
}

/*
** only written when an envelope was added (they never change)
*/
int db_save_envelopes(library_t *lib)
{
    FILE *file = NULL;

    if (!lib->envelopes_changed)
        return 0;
    file = tsv_create(ENVELOPES_DB, ENVELOPES_COLUMNS);
    if (!file)
        return 84;
    for (envelope_t *env = lib->envelopes; env; env = env->next) {
        fprintf(file, "%s\t", env->hash);
        for (int i = 0; i < env->nb; i++)
            fprintf(file, "%02x", env->data[i]);
        fprintf(file, "\n");
    }
    lib->envelopes_changed = 0;
    return tsv_commit(file, ENVELOPES_DB);
}

void db_parse_not_dup(library_t *lib, char **fields, int nb)
{
    if (nb == 2 && atoi(fields[0]) > 0 && atoi(fields[1]) > 0)
        not_dup_add(lib, atoi(fields[0]), atoi(fields[1]));
}

int db_save_not_dups(library_t *lib)
{
    FILE *file = tsv_create(NOT_DUPS_DB, NOT_DUPS_COLUMNS);

    if (!file)
        return 84;
    for (not_dup_t *nd = lib->not_dups; nd; nd = nd->next)
        fprintf(file, "%d\t%d\n", nd->a, nd->b);
    return tsv_commit(file, NOT_DUPS_DB);
}
