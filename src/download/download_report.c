/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** report of a queue: what became of each video (added, already there,
** excluded, refused, put aside, unavailable, failed, interrupted),
** shown at the end or when the queue is stopped, and appended to
** data/downloads.log to be read later
*/

#include "mp3player.h"

static char const *const status_names[NB_REPORTS] = {"added", "present",
    "excluded", "refused", "flagged", "unavailable", "failed",
    "interrupted"};

/*
** appended at the end: the lines keep the order of the queue
*/
void report_add(download_t *dl, report_status_t status, char const *title,
    char const *detail)
{
    dl_report_t **last = &dl->report;
    dl_report_t *line = calloc(1, sizeof(dl_report_t));

    if (!line)
        return;
    line->status = status;
    line->title = strdup(title ? title : "?");
    line->detail = strdup(detail ? detail : "");
    while (*last)
        last = &(*last)->next;
    *last = line;
}

void report_free(download_t *dl)
{
    dl_report_t *next = NULL;

    while (dl->report) {
        next = dl->report->next;
        free(dl->report->title);
        free(dl->report->detail);
        free(dl->report);
        dl->report = next;
    }
}

int report_count(download_t const *dl, report_status_t status)
{
    int nb = 0;

    for (dl_report_t const *line = dl->report; line; line = line->next)
        nb += line->status == status;
    return nb;
}

/*
** the videos still waiting and the links not listed yet are
** "interrupted" when the queue is stopped
*/
static void interrupted(download_t *dl)
{
    if (dl->state == DL_RUNNING && dl->step == DL_STEP_LIST && dl->next > 0)
        report_add(dl, REP_INTERRUPTED, dl->queue[dl->next - 1],
            "liste en cours de lecture");
    if (dl->cur)
        report_add(dl, REP_INTERRUPTED, dl->cur->title, "en cours");
    for (dl_entry_t *e = dl->todo; e; e = e->next)
        report_add(dl, REP_INTERRUPTED, e->title, "pas encore téléchargé");
    for (int i = dl->next; dl->queue && dl->queue[i]; i++)
        report_add(dl, REP_INTERRUPTED, dl->queue[i], "lien pas encore lu");
}

/*
** stopped : 1 if the queue was cancelled (or the app closed)
*/
void report_finish(download_t *dl, int stopped)
{
    char line[DL_LINE * 3];

    if (stopped)
        interrupted(dl);
    for (dl_entry_t *e = dl->flagged; e; e = e->next)
        report_add(dl, REP_FLAGGED, e->title, "hors durée, en attente");
    for (dl_report_t *rep = dl->report; rep; rep = rep->next) {
        snprintf(line, sizeof(line), "%s\t%s\t%s",
            status_names[rep->status], rep->title, rep->detail);
        log_line(DOWNLOADS_LOG, DOWNLOADS_COLUMNS, line);
    }
    dl->show_report = 1;
}
