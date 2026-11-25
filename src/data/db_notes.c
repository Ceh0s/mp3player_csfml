/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes song_notes.tsv (song id, where found, date found)
*/

#include "mp3player.h"

void db_parse_note(library_t *lib, char **fields, int nb)
{
    if (nb != 3 || atoi(fields[0]) <= 0)
        return;
    note_put(lib, atoi(fields[0]), fields[1], fields[2]);
}

int db_save_notes(library_t *lib)
{
    FILE *file = tsv_create(NOTES_DB, NOTES_COLUMNS);

    if (!file)
        return 84;
    for (note_t *note = lib->notes; note; note = note->next)
        fprintf(file, "%d\t%s\t%s\n", note->song, note->found, note->date);
    return tsv_commit(file, NOTES_DB);
}
