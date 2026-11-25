/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** where and when a song was found, typed by the author (edit_notes.c,
** song_notes.tsv): raw ids; a song without a note of its own shows the
** one of a song merged into it
*/

#include "mp3player.h"

static note_t *note_of(library_t *lib, int song)
{
    for (note_t *note = lib->notes; note; note = note->next)
        if (note->song == song)
            return note;
    return NULL;
}

note_t *note_find(library_t *lib, song_t const *song)
{
    int ids[SONG_IDS];
    int nb = song_ids(lib, song, ids);
    note_t *note = NULL;

    for (int i = 0; i < nb && !note; i++)
        note = note_of(lib, ids[i]);
    return note;
}

/*
** the note of this raw id is created or replaced; returns 84 on error
*/
int note_put(library_t *lib, int song, char const *found, char const *date)
{
    note_t *note = note_of(lib, song);
    char *texts[2] = {strdup(found), strdup(date)};

    if (!note && texts[0] && texts[1]) {
        note = calloc(1, sizeof(note_t));
        if (note) {
            note->song = song;
            note->next = lib->notes;
            lib->notes = note;
        }
    }
    if (!note || !texts[0] || !texts[1]) {
        free(texts[0]);
        free(texts[1]);
        return 84;
    }
    free(note->found);
    free(note->date);
    note->found = texts[0];
    note->date = texts[1];
    return 0;
}

static void log_change(song_t const *song, char const *field,
    char const *before, char const *after)
{
    char line[NAME_LEN * 2 + 64];

    if (strcmp(before, after) == 0)
        return;
    snprintf(line, sizeof(line), "%d\t%s\t%s\t%s", song->id, field, before,
        after);
    log_line(EDITS_LOG, EDITS_COLUMNS, line);
}

/*
** saved at once, the changes are written in edits.log (an emptied note
** is kept empty: it hides the one of a merged song)
*/
void note_set(library_t *lib, song_t *song, char const *found,
    char const *date)
{
    note_t *note = note_find(lib, song);
    char const *old_found = note ? note->found : "";
    char const *old_date = note ? note->date : "";

    if (strcmp(old_found, found) == 0 && strcmp(old_date, date) == 0)
        return;
    log_change(song, "found", old_found, found);
    log_change(song, "discovered", old_date, date);
    note_put(lib, song->id, found, date);
    db_save_notes(lib);
}

/*
** song = 0 frees all of them
*/
void notes_remove(library_t *lib, int song)
{
    note_t **cur = &lib->notes;
    note_t *old = NULL;

    while (*cur) {
        if (song != 0 && (*cur)->song != song) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old->found);
        free(old->date);
        free(old);
    }
}
