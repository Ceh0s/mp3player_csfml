/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** videos found by the listing of a link: each line written by yt-dlp
** with DL_FORMAT ("id, duration, live status, address, channel, title"
** separated by tabs; the channel is "NA" when unknown) becomes an
** entry; the lists of entries (listed, todo, flagged) are handled here
*/

#include "mp3player.h"

void entries_free(dl_entry_t *list)
{
    dl_entry_t *next = NULL;

    while (list) {
        next = list->next;
        free(list->url);
        free(list->channel);
        free(list->title);
        free(list);
        list = next;
    }
}

void entry_append(dl_entry_t **list, dl_entry_t *entry)
{
    while (*list)
        list = &(*list)->next;
    entry->next = NULL;
    *list = entry;
}

dl_entry_t *entry_find(dl_entry_t *list, char const *id)
{
    for (; list; list = list->next)
        if (strcmp(list->id, id) == 0)
            return list;
    return NULL;
}

dl_entry_t *entry_pop(dl_entry_t **list)
{
    dl_entry_t *entry = *list;

    if (entry) {
        *list = entry->next;
        entry->next = NULL;
    }
    return entry;
}

/*
** a duration is written "597", "597.0", or "NA" when the site does not
** give it (-1: unknown); a live stream being broadcast, about to start or
** just ended cannot be downloaded whole
*/
static dl_entry_t *entry_new(char **fields, dl_options_t const *opt)
{
    dl_entry_t *entry = calloc(1, sizeof(dl_entry_t));

    if (!entry)
        return NULL;
    snprintf(entry->id, VIDEO_ID, "%s", fields[0]);
    entry->duration = fields[1][0] >= '0' && fields[1][0] <= '9' ?
        (int)atof(fields[1]) : -1;
    entry->live = strcmp(fields[2], "is_live") == 0
        || strcmp(fields[2], "is_upcoming") == 0
        || strcmp(fields[2], "post_live") == 0;
    entry->url = strdup(fields[3]);
    entry->channel = strdup(strcmp(fields[4], "NA") == 0 ? "" : fields[4]);
    entry->title = strdup(fields[5]);
    memcpy(entry->targets, opt->targets, sizeof(entry->targets));
    entry->nb_targets = opt->nb_targets;
    if (!entry->url || !entry->channel || !entry->title) {
        entries_free(entry);
        return NULL;
    }
    return entry;
}

/*
** returns 1 if the line describes a video (it is then added to listed),
** 0 for any other line written by yt-dlp (warning, error...)
*/
int download_list_line(download_t *dl, char const *line)
{
    char copy[DL_LINE];
    char *fields[7];
    dl_entry_t *entry = NULL;

    snprintf(copy, sizeof(copy), "%s", line);
    if (split_tab(copy, fields, 7) != 6 || fields[0][0] == '\0'
        || strlen(fields[0]) >= VIDEO_ID || strchr(fields[0], ' ')
        || strncmp(fields[3], "http", 4) != 0)
        return 0;
    entry = entry_new(fields, &dl->opt);
    if (entry)
        entry_append(&dl->listed, entry);
    return 1;
}
