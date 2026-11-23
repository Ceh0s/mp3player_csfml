/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** once a link is listed, each of its videos is sorted:
** - already in music/ : not downloaded, only added to the playlists
** - excluded for good (excluded.tsv) : left out
** - private, deleted or live stream : left out
** - out of the allowed duration : left out if it was already refused,
**   else put aside (flagged) until a decision
** - else : to download (a refused video back in the allowed duration
**   is no longer refused)
*/

#include "mp3player.h"

static int out_of_range(dl_options_t const *opt, int duration)
{
    if (duration < 0)
        return 0;
    if (opt->min_s >= 0 && duration < opt->min_s)
        return 1;
    return opt->max_s >= 0 && duration > opt->max_s;
}

/*
** yt-dlp lists them as "[Private video]", "[Deleted video]"...
*/
static int unavailable(dl_entry_t const *entry)
{
    size_t len = strlen(entry->title);

    if (entry->live)
        return 1;
    return entry->title[0] == '[' && len > 7
        && strcmp(entry->title + len - 7, " video]") == 0;
}

/*
** returns 1 if the entry was kept (put aside or to download)
*/
static int keep_entry(app_t *app, download_t *dl, dl_entry_t *entry)
{
    if (entry_find(dl->todo, entry->id) || entry_find(dl->flagged,
            entry->id) || (dl->cur && strcmp(dl->cur->id, entry->id) == 0))
        return 0;
    if (!out_of_range(&dl->opt, entry->duration)) {
        rejected_remove(app->lib, entry->id);
        entry_append(&dl->todo, entry);
        dl->items++;
        return 1;
    }
    if (rejected_find(app->lib, entry->id)) {
        dl->refused++;
        report_add(dl, REP_REFUSED, entry->title, "hors durée, refusé avant");
        return 0;
    }
    entry_append(&dl->flagged, entry);
    return 1;
}

/*
** what is left out without a decision: present, excluded, unavailable
*/
static int left_out(app_t *app, download_t *dl, dl_entry_t *entry,
    song_t *song)
{
    if (song) {
        playlists_append(app->lib, entry->targets, entry->nb_targets, song);
        dl->skipped++;
        report_add(dl, REP_PRESENT, entry->title, "");
        return 1;
    }
    if (excluded_find(app->lib, entry->id)) {
        dl->excluded++;
        report_add(dl, REP_EXCLUDED, entry->title, "");
        return 1;
    }
    if (!unavailable(entry))
        return 0;
    dl->unavailable++;
    report_add(dl, REP_UNAVAILABLE, entry->title,
        entry->live ? "live" : "privée ou supprimée");
    return 1;
}

static void sort_entry(app_t *app, download_t *dl, dl_entry_t *entry)
{
    song_t *song = song_by_video(app->lib, entry->id);

    if (!left_out(app, dl, entry, song) && keep_entry(app, dl, entry))
        return;
    entries_free(entry);
}

void download_sort(app_t *app)
{
    download_t *dl = &app->download;
    dl_entry_t *entry = entry_pop(&dl->listed);

    while (entry) {
        sort_entry(app, dl, entry);
        entry = entry_pop(&dl->listed);
    }
    db_save_playlists(app->lib);
}
