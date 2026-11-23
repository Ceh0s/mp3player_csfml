/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** pages "Bilan" (what became of each video of the last queue, grouped
** by status), "Refusés" and "Exclus" (videos refused for their duration
** or never to download again; both can be cancelled) of the list over
** the download screen
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
    widget_text(app, dl->report ? line : "Aucun téléchargement pour "
        "l'instant.", (float [3]){300, 265, 22}, &sfWhite);
}

static void report_row(app_t *app, dl_report_t const *line, float y)
{
    char cut[64 + 4];

    widget_text(app, labels[line->status], (float [3]){300, y + 8, 22},
        &colors[line->status]);
    utf8_cut(cut, line->title, 64);
    widget_text(app, cut, (float [3]){480, y + 8, 22}, &sfWhite);
    utf8_cut(cut, line->detail, 40);
    widget_text(app, cut, (float [3]){1200, y + 8, 20}, &gray);
}

/*
** the lines of one status; i counts the lines before (for the scroll)
*/
static void status_rows(app_t *app, report_status_t status, int *i)
{
    int first = app->dlscene->scroll;
    int rows = dlpopup_rows(app);

    for (dl_report_t *line = app->download.report; line; line = line->next) {
        if (line->status != status)
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

/*
** the list changes after a decision: the loop stops there
*/
void dlscene_refused(app_t *app)
{
    int first = app->dlscene->scroll;
    int i = -1;
    float y = DLPOPUP_ROWS_Y - DLPOPUP_STEP;
    char text[80 + 4];

    for (rejected_t *rej = app->lib->rejected; rej; rej = rej->next) {
        i++;
        if (i < first || i - first >= dlpopup_rows(app))
            continue;
        y += DLPOPUP_STEP;
        utf8_cut(text, rej->title, 70);
        widget_text(app, text, (float [3]){300, y + 8, 22}, &sfWhite);
        format_time(text, sizeof(text), rej->duration * 1000L);
        widget_text(app, text, (float [3]){1070, y + 8, 22}, &gray);
        if (widget_button(app, (float [4]){1405, y, 250, 44}, "Reprendre")) {
            rejected_remove(app->lib, rej->id);
            db_save_rejected(app->lib);
            return;
        }
    }
}

int dlscene_report_lines(app_t *app)
{
    int nb = 0;

    for (dl_report_t *line = app->download.report; line; line = line->next)
        nb++;
    return nb;
}

/*
** the list changes after a decision: the loop stops there
*/
void dlscene_excluded(app_t *app)
{
    library_t *lib = app->lib;
    int first = app->dlscene->scroll;
    int i = -1;
    float y = DLPOPUP_ROWS_Y - DLPOPUP_STEP;
    char cut[80 + 4];

    for (rejected_t *ex = lib->excluded; ex; ex = ex->next) {
        i++;
        if (i < first || i - first >= dlpopup_rows(app))
            continue;
        y += DLPOPUP_STEP;
        utf8_cut(cut, ex->title, 80);
        widget_text(app, cut, (float [3]){300, y + 8, 22}, &sfWhite);
        if (widget_button(app, (float [4]){1405, y, 250, 44}, "Reprendre")) {
            excluded_remove(lib, ex->id);
            db_save_excluded(lib);
            return;
        }
    }
}
