/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** "Oublier" an excluded song (download screen, list "Exclus"): unlike
** the rest of the app, where nothing is ever removed, everything about
** the song goes (asked twice by the author: excluded songs are unwanted,
** their stats are not worth keeping): the song, its fingerprints and
** sound envelopes, its references (song_forget_refs.c), its lines in
** the logs and its images (song_forget_logs.c). Only its video stays in
** excluded.tsv, so it is never downloaded again. Its id is never given
** again (next_song is kept in settings.tsv). A line "forget" is written
** in edits.log.
*/

#include "mp3player.h"

/*
** 1 if the song can be forgotten: its file is gone, and no other song
** was merged into it (their stats would go with it)
*/
int song_forgettable(library_t *lib, song_t const *song)
{
    if (!song || song->present)
        return 0;
    for (song_t const *other = lib->songs; other; other = other->next)
        if (other->merged_into == song->id)
            return 0;
    return 1;
}

static void forget_envelope(library_t *lib, char const *hash)
{
    envelope_t **cur = &lib->envelopes;
    envelope_t *old = NULL;

    while (*cur && strcmp((*cur)->hash, hash) != 0)
        cur = &(*cur)->next;
    if (!*cur)
        return;
    old = *cur;
    *cur = old->next;
    free(old->hash);
    free(old->data);
    free(old);
    lib->envelopes_changed = 1;
}

static void forget_fprints(library_t *lib, int id)
{
    fprint_t **cur = &lib->fprints;
    fprint_t *old = NULL;

    while (*cur) {
        if ((*cur)->song_id != id) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        forget_envelope(lib, old->hash);
        free(old->hash);
        free(old->path);
        free(old->file);
        free(old->source);
        free(old);
    }
}

static void forget_node(library_t *lib, song_t *song)
{
    song_t **cur = &lib->songs;

    while (*cur && *cur != song)
        cur = &(*cur)->next;
    if (!*cur)
        return;
    *cur = song->next;
    song_destroy(song);
}

/*
** returns 84 if the song cannot be forgotten (song_forgettable)
*/
int song_forget(app_t *app, song_t *song)
{
    char line[NAME_LEN + 64];
    int id = 0;

    if (!song_forgettable(app->lib, song))
        return 84;
    id = song->id;
    snprintf(line, sizeof(line), "%d\tforget\t%s\t(oublié)", id,
        song_title(song));
    log_line(EDITS_LOG, EDITS_COLUMNS, line);
    song_forget_logs(song);
    song_forget_refs(app->lib, id);
    forget_fprints(app->lib, id);
    forget_node(app->lib, song);
    library_changed(app);
    db_save_settings(app->lib);
    return 0;
}
