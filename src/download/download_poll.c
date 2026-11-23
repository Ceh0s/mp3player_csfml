/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** follows the download at each tick: reads what yt-dlp wrote (without
** waiting, the pipe is non blocking) and checks if it ended (waitpid
** WNOHANG). A listing writes one line per video; a download writes its
** progress in lines "[download]  42.3% of ...".
*/

#include "mp3player.h"
#include <sys/wait.h>

static void end_line(download_t *dl)
{
    float percent = 0;

    dl->line[dl->len] = '\0';
    dl->len = 0;
    if (dl->line[0] == '\0')
        return;
    if (dl->step == DL_STEP_LIST && download_list_line(dl, dl->line)) {
        dl->found++;
        snprintf(dl->last, DL_LINE, "Lecture de la liste : %d vidéo(s) "
            "trouvée(s)", dl->found);
        return;
    }
    if (sscanf(dl->line, "[download] %f%%", &percent) == 1)
        dl->percent = percent;
    stats_parse(&dl->stats, dl->line);
    snprintf(dl->last, DL_LINE, "%s", dl->line);
}

/*
** yt-dlp may end a line with \r or \n; a too long line is cut
*/
static void add_char(download_t *dl, char c)
{
    if (c == '\n' || c == '\r') {
        end_line(dl);
        return;
    }
    if (dl->len >= DL_LINE - 1)
        return;
    dl->line[dl->len] = c;
    dl->len++;
}

static void read_output(download_t *dl)
{
    char buf[512];
    ssize_t size = read(dl->fd, buf, sizeof(buf));

    while (size > 0) {
        for (ssize_t i = 0; i < size; i++)
            add_char(dl, buf[i]);
        size = read(dl->fd, buf, sizeof(buf));
    }
}

/*
** a listing that failed may still have found videos (a playlist with
** an error on one of them): they are downloaded anyway
*/
/*
** the report keeps the video (or the link) that failed, with the reason
*/
static void failed(download_t *dl)
{
    dl->failed++;
    snprintf(dl->error, DL_LINE, "%s", dl->last);
    if (dl->step == DL_STEP_LIST && dl->next > 0)
        report_add(dl, REP_FAILED, dl->queue[dl->next - 1], dl->last);
    if (dl->step == DL_STEP_GET && dl->cur)
        report_add(dl, REP_FAILED, dl->cur->title, dl->last);
}

/*
** a video that YouTube does not serve is not tried again next time
*/
static void remember(app_t *app, download_t *dl)
{
    if (dl->step != DL_STEP_GET || !dl->cur || !dl_reason_lasting(dl->last))
        return;
    unavailable_add(app->lib, dl->cur->id, dl->cur->title, dl->last);
    db_save_unavailable(app->lib);
}

/*
** a video no longer served whose new version is in the library is not
** a failure: that song goes to the playlists instead
*/
static void not_done(app_t *app, download_t *dl)
{
    int lasting = dl->step == DL_STEP_GET && dl->cur
        && dl_reason_lasting(dl->last);

    remember(app, dl);
    if (lasting && download_reupload(app, dl->cur)) {
        db_save_playlists(app->lib);
        return;
    }
    failed(dl);
}

static void step_done(app_t *app, download_t *dl, int success)
{
    if (!success)
        not_done(app, dl);
    if (dl->step == DL_STEP_LIST) {
        if (success && dl->found > 0)
            download_find_absent(app);
        dl->found = 0;
        download_sort(app);
        return;
    }
    stats_item_done(&dl->stats);
    if (!success)
        return;
    dl->ok++;
    dl->percent = 100;
    report_add(dl, REP_ADDED, dl->cur->title, "");
    download_added(app, dl->cur);
}

static void finish(app_t *app, int status)
{
    download_t *dl = &app->download;

    read_output(dl);
    end_line(dl);
    close(dl->fd);
    dl->state = DL_IDLE;
    step_done(app, dl, WIFEXITED(status) && WEXITSTATUS(status) == 0);
    download_next(app);
}

void download_poll(app_t *app)
{
    download_t *dl = &app->download;
    int status = 0;

    if (dl->state != DL_RUNNING)
        return;
    dl->stats.elapsed_us += app->delta_us;
    read_output(dl);
    if (waitpid(dl->pid, &status, WNOHANG) == dl->pid)
        finish(app, status);
}
