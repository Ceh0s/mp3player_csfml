/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page "Bilan" of the list over the download screen: what became of
** each video of the last queue, grouped by status
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static char const *const labels[NB_REPORTS] = {"Ajouté", "Déjà là",
    "Exclu", "Refusé", "En attente", "Indisponible", "Échec",
    "Interrompu"};

static const sfColor colors[NB_REPORTS] = {
    {130, 230, 130, 255}, {180, 180, 180, 255}, {200, 150, 255, 255},
    {255, 180, 100, 255}, {255, 220, 90, 255}, {180, 180, 180, 255},
    {255, 110, 110, 255}, {255, 140, 140, 255}
};

/*
** "Ajouté 3   Déjà là 140   ..." (only the statuses that happened)
*/
static void summary(app_t *app, download_t const *dl)
{
    char line[256];
    int len = 0;
    int nb = 0;

    line[0] = '\0';
    for (int s = 0; s < NB_REPORTS; s++) {
        nb = report_count(dl, s);
        if (nb > 0)
            len += snprintf(line + len, sizeof(line) - len, "%s %d     ",
                labels[s], nb);
    }
    widget_text(app, dl->report ? line : "Aucun téléchargement.",
        (float [3]){300, 265, 22}, &sfWhite);
}

/*
** the reason is said in French when it is known; the whole line of
** yt-dlp is shown when the mouse is over the row
*/
static void report_row(app_t *app, dl_report_t const *line, float y)
{
    float row[4] = {300, y, 1355, DLPOPUP_STEP - 4};
    char cut[64 + 4];
    char reason[DL_LINE];

    widget_text(app, labels[line->status], (float [3]){300, y + 8, 22},
        &colors[line->status]);
    utf8_cut(cut, line->title, 48);
    widget_text(app, cut, (float [3]){480, y + 8, 22}, &sfWhite);
    dl_reason(reason, sizeof(reason), line->detail);
    utf8_cut(cut, reason, 60);
    widget_text(app, cut, (float [3]){1060, y + 8, 20}, &gray);
    if (line->detail[0] != '\0')
        widget_tip(app, row, line->detail);
}

/*
** the lines of one status; i counts the lines before (for the scroll)
*/
static void status_rows(app_t *app, report_status_t status, int *i)
{
    int first = app->dlscene->scroll;
    int rows = dlpopup_rows(app);

    for (dl_report_t *line = app->download.report; line; line = line->next) {
        if (line->status != status || !search_text(app, line->title))
            continue;
        if (*i >= first && *i - first < rows)
            report_row(app, line, DLPOPUP_ROWS_Y + (*i - first)
                * DLPOPUP_STEP);
        (*i)++;
    }
}

/*
** the lines are shown grouped by status, in the order of report_status_t
*/
void dlscene_report(app_t *app)
{
    int i = 0;

    summary(app, &app->download);
    for (int s = 0; s < NB_REPORTS; s++)
        status_rows(app, s, &i);
}

int dlscene_report_lines(app_t *app)
{
    int nb = 0;

    for (dl_report_t *line = app->download.report; line; line = line->next)
        nb++;
    return nb;
}
