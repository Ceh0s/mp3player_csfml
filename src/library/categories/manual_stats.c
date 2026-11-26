/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** stats set by hand on the page of a song ("Biais de récence"): kept by
** raw id, so a merge shows the value of the song merged into, and a
** split gives each one its own back
*/

#include "mp3player.h"

static manual_t *manual_find(library_t *lib, int song_id, int stat)
{
    for (manual_t *m = lib->manual; m; m = m->next)
        if (m->song_id == song_id && m->stat == stat)
            return m;
    return NULL;
}

static void manual_remove(library_t *lib, int song_id, int stat)
{
    manual_t **link = &lib->manual;
    manual_t *m = NULL;

    while (*link) {
        m = *link;
        if (m->song_id == song_id && (stat < 0 || m->stat == stat)) {
            *link = m->next;
            free(m);
        } else
            link = &m->next;
    }
}

/*
** value < 0 removes it
*/
void manual_set(library_t *lib, int song_id, int stat, int value)
{
    manual_t *m = manual_find(lib, song_id, stat);

    if (value < 0) {
        manual_remove(lib, song_id, stat);
        return;
    }
    if (value > 100)
        value = 100;
    if (!m) {
        m = calloc(1, sizeof(manual_t));
        if (!m)
            return;
        m->song_id = song_id;
        m->stat = stat;
        m->next = lib->manual;
        lib->manual = m;
    }
    m->value = value;
}

/*
** the value of the song itself, else of a song merged into it; -1 if none
*/
int manual_get(library_t *lib, song_t *song, int stat)
{
    manual_t *own = manual_find(lib, song->id, stat);

    if (own)
        return own->value;
    for (manual_t *m = lib->manual; m; m = m->next)
        if (m->stat == stat
            && song_resolve(lib, song_by_id(lib, m->song_id)) == song)
            return m->value;
    return -1;
}

/*
** a forgotten song (song_forget.c) loses its values; -1 frees them all
*/
void manual_forget(library_t *lib, int song_id)
{
    manual_t *next = NULL;

    if (song_id >= 0) {
        manual_remove(lib, song_id, -1);
        return;
    }
    for (manual_t *m = lib->manual; m; m = next) {
        next = m->next;
        free(m);
    }
    lib->manual = NULL;
}
