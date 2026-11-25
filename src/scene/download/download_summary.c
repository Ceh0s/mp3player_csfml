/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** sum up of the download screen once a queue is over (or failed): songs
** downloaded, size and time, and the videos left out (already in music/,
** refused, unavailable, put aside because of their duration)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor green = {120, 220, 120, 255};
static const sfColor orange = {255, 160, 60, 255};

/*
** adds ", <nb> <text>" (or "<nb> <text>" at the start) when nb > 0
*/
static int add_part(char *buf, int len, int nb, char const *text)
{
    if (nb <= 0 || len >= 300)
        return len;
    return len + snprintf(buf + len, 300 - len, "%s%d %s",
        len > 0 ? ", " : "", nb, text);
}

static void left_out(app_t *app, download_t const *dl)
{
    char line[300];
    int len = 0;

    line[0] = '\0';
    len = add_part(line, len, dl->skipped, "déjà présente(s)");
    len = add_part(line, len, entries_count(dl->flagged), "hors durée");
    len = add_part(line, len, dl->refused, "déjà refusée(s)");
    len = add_part(line, len, dl->excluded, "exclue(s)");
    add_part(line, len, dl->unavailable, "indisponible(s)");
    if (line[0] != '\0')
        widget_text(app, line, (float [3]){130, 645, 22}, &sfWhite);
}

/*
** "3 échec(s), le dernier : <reason>" (in French when it is known, the
** whole line of yt-dlp is in the report), also while the queue runs
*/
void dlscene_failures(app_t *app, download_t const *dl, float y)
{
    char reason[DL_LINE];
    char line[DL_LINE + 64];

    if (dl->failed <= 0)
        return;
    dl_reason(reason, sizeof(reason), dl->error);
    snprintf(line, sizeof(line), "%d échec(s), le dernier : %s",
        dl->failed, reason);
    widget_text(app, line, (float [3]){130, y, 22}, &orange);
}

static void done_lines(app_t *app, download_t const *dl)
{
    char line[160];
    char size[32];
    char time[32];

    format_size(size, sizeof(size), dl->stats.bytes);
    format_time(time, sizeof(time), dl->stats.elapsed_us / 1000);
    snprintf(line, sizeof(line), "Terminé : %d titre(s) téléchargé(s), "
        "%s en %s", dl->ok, size, time);
    widget_text(app, line, (float [3]){130, 600, 26}, &green);
    left_out(app, dl);
    dlscene_failures(app, dl, 700);
}

/*
** the duplicates screen then only looks at the songs just downloaded
*/
static void duplicates_button(app_t *app, download_t const *dl)
{
    if (!widget_button(app, (float [4]){420, 840 + app->extra_h, 640, 56},
            "Chercher des doublons parmi les nouveaux titres"))
        return;
    app->dup_since = dl->started;
    scene_open(app, DUPLICATES);
}

/*
** the report of the last queue, also after a failure or a cancel
*/
static void report_button(app_t *app, download_t const *dl)
{
    if (!dl->report || dl->state == DL_RUNNING
        || !widget_button(app, (float [4]){1080, 840 + app->extra_h, 300, 56},
            "Voir le bilan"))
        return;
    app->dlscene->popup = DL_POPUP_REPORT;
    app->dlscene->scroll = 0;
}

void dlscene_status(app_t *app, download_t const *dl)
{
    report_button(app, dl);
    if (dl->state == DL_DONE)
        done_lines(app, dl);
    if (dl->state == DL_DONE && dl->ok > 0)
        duplicates_button(app, dl);
    if (dl->state != DL_FAILED)
        return;
    widget_text(app, "Échec du téléchargement :",
        (float [3]){130, 600, 26}, &orange);
    widget_text(app, dl->last, (float [3]){130, 645, 20}, &gray);
}
