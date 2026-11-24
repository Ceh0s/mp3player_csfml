/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** videos excluded for good (a meme, a short liked by mistake...): never
** downloaded again, whatever the duration (excluded.tsv); excluding a
** song deletes its file (its stats are kept)
*/

#include "mp3player.h"

rejected_t *excluded_find(library_t *lib, char const *id)
{
    for (rejected_t *ex = lib->excluded; ex; ex = ex->next)
        if (strcmp(ex->id, id) == 0)
            return ex;
    return NULL;
}

/*
** added at the end of the list; an id already there stays as it is
*/
rejected_t *excluded_add(library_t *lib, char const *id, char const *title)
{
    rejected_t **last = &lib->excluded;
    rejected_t *ex = excluded_find(lib, id);

    if (ex)
        return ex;
    ex = calloc(1, sizeof(rejected_t));
    if (!ex)
        return NULL;
    snprintf(ex->id, VIDEO_ID, "%s", id);
    ex->title = strdup(title ? title : "");
    ex->duration = -1;
    ex->song_id = -1;
    ex->date = time(NULL);
    while (*last)
        last = &(*last)->next;
    *last = ex;
    return ex;
}

/*
** id = NULL frees all of them
*/
void excluded_remove(library_t *lib, char const *id)
{
    rejected_t **cur = &lib->excluded;
    rejected_t *old = NULL;

    while (*cur) {
        if (id && strcmp((*cur)->id, id) != 0) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old->title);
        free(old);
    }
}

/*
** a song can only be excluded if it came from a video (id known);
** returns 84 if it did not
*/
int song_exclude(app_t *app, song_t *song)
{
    char id[VIDEO_ID];
    rejected_t *ex = NULL;

    if (!song_video_id(app->lib, song, id))
        return 84;
    ex = excluded_add(app->lib, id, song_title(song));
    if (ex)
        ex->song_id = song->id;
    db_save_excluded(app->lib);
    song_delete_files(app, song);
    return 0;
}

/*
** archived : 1 to count those of "Voir les archivés", 0 the others
*/
int excluded_count(library_t const *lib, int archived)
{
    int nb = 0;

    for (rejected_t const *ex = lib->excluded; ex; ex = ex->next)
        nb += ex->archived == archived;
    return nb;
}
