/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** queue of links: the pasted text is cut on ; (and spaces). Each link
** is listed, then its videos are downloaded one after the other, a
** single yt-dlp at a time, before the next link is listed. A failure is
** counted and the queue goes on.
*/

#include "mp3player.h"
#include <signal.h>
#include <sys/wait.h>

/*
** the queue is freed when it is over: it failed if nothing was found
** and something failed
*/
static int end_queue(download_t *dl)
{
    report_finish(dl, 0);
    str_array_free(dl->queue);
    dl->queue = NULL;
    dl->state = DL_DONE;
    if (dl->ok == 0 && dl->skipped == 0 && dl->failed > 0) {
        dl->state = DL_FAILED;
        snprintf(dl->last, DL_LINE, "%s", dl->error);
    }
    dl->ask = dl->flagged != NULL || dl->nb_absent > 0;
    return 0;
}

/*
** the videos of the link already listed come first, then the next link
** is listed; NULL when everything is done
*/
static char const *next_url(download_t *dl, dl_step_t *step)
{
    entries_free(dl->cur);
    dl->cur = entry_pop(&dl->todo);
    if (dl->cur) {
        dl->item++;
        *step = DL_STEP_GET;
        return dl->cur->url;
    }
    if (!dl->queue[dl->next])
        return NULL;
    dl->next++;
    *step = DL_STEP_LIST;
    return dl->queue[dl->next - 1];
}

/*
** if yt-dlp cannot even be launched (not installed, no more process),
** the next videos would fail the same way: the queue stops
*/
int download_next(app_t *app)
{
    download_t *dl = &app->download;
    dl_step_t step = DL_STEP_LIST;
    char const *url = NULL;

    if (!dl->queue)
        return 84;
    url = next_url(dl, &step);
    if (!url)
        return end_queue(dl);
    if (download_launch(app, url, step) == 0)
        return 0;
    download_cancel(app);
    dl->state = DL_FAILED;
    return 84;
}

/*
** the address is linked to the playlist, with the options of the queue
*/
static void download_link(library_t *lib, char const *url,
    dl_options_t const *opt)
{
    source_t *src = source_set(lib, opt->targets[0], url);

    if (!src)
        return;
    src->min_s = opt->min_s;
    src->max_s = opt->max_s;
    src->absent = opt->absent;
    db_save_sources(lib);
}

static void reset_counts(download_t *dl)
{
    dl->next = 0;
    dl->item = 0;
    dl->items = 0;
    dl->found = 0;
    dl->ok = 0;
    dl->failed = 0;
    dl->skipped = 0;
    dl->refused = 0;
    dl->excluded = 0;
    dl->unavailable = 0;
    dl->error[0] = '\0';
    memset(&dl->stats, 0, sizeof(dl_stats_t));
    dl->stats.speed = -1;
    dl->stats.cur_eta = -1;
}

/*
** queue : the links; it may be empty when the queue only downloads
** videos already in todo (accepted after being put aside)
*/
int download_begin(app_t *app, char **queue, dl_options_t const *opt)
{
    download_t *dl = &app->download;

    report_free(dl);
    dl->show_report = 0;
    dl->queue = queue;
    dl->opt = *opt;
    dl->started = time(NULL);
    dl->total = 0;
    while (dl->queue[dl->total])
        dl->total++;
    reset_counts(dl);
    for (dl_entry_t *entry = dl->todo; entry; entry = entry->next)
        dl->items++;
    download_archive_write(app->lib);
    return download_next(app);
}

/*
** returns 84 if a download is already running or the text has no link
*/
int download_start(app_t *app, char const *text, dl_options_t const *opt)
{
    download_t *dl = &app->download;
    char **queue = NULL;

    if (dl->queue || dl->state == DL_RUNNING)
        return 84;
    queue = stwa_delimiter(text, "; \t", 0, 0);
    if (!queue || !queue[0]) {
        str_array_free(queue);
        return 84;
    }
    if (opt->link && !queue[1] && opt->nb_targets == 1)
        download_link(app->lib, queue[0], opt);
    return download_begin(app, queue, opt);
}

/*
** stops yt-dlp and ffmpeg (its whole process group) and waits for it,
** so no process is left behind (also called when the app closes); the
** links and videos still waiting are forgotten (written in the report
** as interrupted), not those put aside
*/
void download_cancel(app_t *app)
{
    download_t *dl = &app->download;

    if (dl->queue)
        report_finish(dl, 1);
    str_array_free(dl->queue);
    dl->queue = NULL;
    entries_free(dl->listed);
    entries_free(dl->todo);
    entries_free(dl->cur);
    dl->listed = NULL;
    dl->todo = NULL;
    dl->cur = NULL;
    if (dl->state != DL_RUNNING)
        return;
    kill(-dl->pid, SIGTERM);
    waitpid(dl->pid, NULL, 0);
    close(dl->fd);
    dl->state = DL_FAILED;
    snprintf(dl->last, DL_LINE, "Téléchargement annulé");
}
