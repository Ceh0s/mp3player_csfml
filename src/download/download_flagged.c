/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** decision on a video put aside because of its duration: accepted, it
** is downloaded (added to the running queue, or starting a new one)
** and no longer refused; refused, it is saved in rejected.tsv and left
** out next times while it stays out of the allowed duration (for good
** if it was refused before and is now in the allowed duration)
*/

#include "mp3player.h"

/*
** takes the entry out of the list without freeing it
*/
static void entry_unlink(dl_entry_t **list, dl_entry_t *entry)
{
    while (*list && *list != entry)
        list = &(*list)->next;
    if (*list)
        *list = entry->next;
    entry->next = NULL;
}

/*
** with no queue running, a queue without any link starts: it only has
** this video to download (its playlists are those it was found for)
*/
void download_accept(app_t *app, dl_entry_t *entry)
{
    download_t *dl = &app->download;
    dl_options_t opt = {{0}, 0, -1, -1, 0, 0};
    char **queue = NULL;

    entry_unlink(&dl->flagged, entry);
    entry_append(&dl->todo, entry);
    if (entry->was_refused) {
        rejected_remove(app->lib, entry->id);
        db_save_rejected(app->lib);
    }
    if (dl->queue) {
        dl->items++;
        return;
    }
    queue = calloc(1, sizeof(char *));
    if (!queue) {
        entries_free(entry_pop(&dl->todo));
        return;
    }
    download_begin(app, queue, &opt);
}

void download_refuse(app_t *app, dl_entry_t *entry)
{
    download_t *dl = &app->download;
    rejected_t *rej = NULL;

    entry_unlink(&dl->flagged, entry);
    rej = rejected_add(app->lib, entry->id, entry->title, entry->duration);
    if (rej && entry->was_refused)
        rej->in_range = 1;
    entries_free(entry);
    db_save_rejected(app->lib);
}

/*
** never downloaded again, whatever the duration
*/
void download_exclude(app_t *app, dl_entry_t *entry)
{
    download_t *dl = &app->download;

    entry_unlink(&dl->flagged, entry);
    excluded_add(app->lib, entry->id, entry->title);
    entries_free(entry);
    db_save_excluded(app->lib);
}

/*
** accept = 1 downloads all the videos put aside, 0 refuses them
*/
void download_decide_all(app_t *app, int accept)
{
    download_t *dl = &app->download;

    while (dl->flagged) {
        if (accept)
            download_accept(app, dl->flagged);
        else
            download_refuse(app, dl->flagged);
    }
}

int entries_count(dl_entry_t const *list)
{
    int nb = 0;

    for (; list; list = list->next)
        nb++;
    return nb;
}
