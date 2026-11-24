/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** once a link is listed, each of its videos is sorted:
** - already in music/ : not downloaded, only added to the playlists
** - excluded for good (excluded.tsv) : left out
** - not served by YouTube the last time (unavailable.tsv) : left out
** - private, deleted or live stream : left out
** - out of the allowed duration : left out if it was already refused,
**   else put aside (flagged) until a decision
** - refused before and now in the allowed duration : put aside too (the
**   user confirms), left out if it was refused again in the duration
** - else : to download
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
** a video that already failed for good (unavailable.tsv) is not tried
** again: the report gives the reason it failed
*/
static int known_unavailable(app_t *app, download_t *dl,
    dl_entry_t const *entry)
{
    rejected_t *un = unavailable_find(app->lib, entry->id);
    char reason[DL_LINE];

    if (!un)
        return 0;
    if (download_reupload(app, entry))
        return 1;
    dl->unavailable++;
    dl_reason(reason, sizeof(reason), un->reason);
    report_add(dl, REP_UNAVAILABLE, entry->title, reason);
    return 1;
}

/*
** a refused video is left out while it stays out of the duration, or
** for good if it was refused again in the duration; returns 1 if so
*/
static int still_refused(download_t *dl, rejected_t const *rej, int out)
{
    if (!rej || (!out && !rej->in_range))
        return 0;
    dl->refused++;
    report_add(dl, REP_REFUSED, rej->title, out ? "hors durée, refusé "
        "avant" : "refusé avant, aussi dans la durée");
    return 1;
}

/*
** returns 1 if the entry was kept (put aside or to download)
*/
static int keep_entry(app_t *app, download_t *dl, dl_entry_t *entry)
{
    rejected_t *rej = rejected_find(app->lib, entry->id);
    int out = out_of_range(&dl->opt, entry->duration);

    if (entry_find(dl->todo, entry->id) || entry_find(dl->flagged,
            entry->id) || (dl->cur && strcmp(dl->cur->id, entry->id) == 0)
        || still_refused(dl, rej, out))
        return 0;
    if (!out && !rej) {
        entry_append(&dl->todo, entry);
        dl->items++;
        return 1;
    }
    entry->was_refused = rej != NULL;
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
        report_add(dl, REP_PRESENT, entry->title, swap_detail(entry));
        return 1;
    }
    if (excluded_find(app->lib, entry->id)) {
        dl->excluded++;
        report_add(dl, REP_EXCLUDED, entry->title, "");
        return 1;
    }
    if (known_unavailable(app, dl, entry))
        return 1;
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
    dl_entry_t *entry = NULL;

    download_keep_order(app);
    entry = entry_pop(&dl->listed);
    while (entry) {
        sort_entry(app, dl, entry);
        entry = entry_pop(&dl->listed);
    }
    db_save_playlists(app->lib);
}
