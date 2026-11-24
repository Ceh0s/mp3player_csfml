/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list shown over the download screen, in seven pages: the videos put
** aside because of their duration (download, refuse or exclude each
** one, or all of them), the videos refused before (a refusal can be
** cancelled), the songs no longer in the playlist of the site
** (download_absent_view.c), the excluded videos and the report of the
** last queue (download_report_view.c), the unavailable videos
** (download_unavailable_view.c), the artist names to check
** (download_split_view.c); the search field filters the
** rows of every page by title
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static char const *const help[] = {"",
    "Hors de la durée choisie, ou refusés avant et revenus dans la durée"
    " : en attente.",
    "Ignorés hors de la durée ; dans la durée, redemandés (ignorés pour "
    "de bon si refusés encore).",
    "Dans la playlist locale, mais plus dans celle du site.",
    "Jamais téléchargés à nouveau, quelle que soit la durée.",
    "Ce qu'est devenue chaque vidéo de la dernière file.",
    "Anciennes versions de titres republiés : ignorées aux prochains "
    "téléchargements.",
    "Artistes écrits en liste qui sont peut-être un seul artiste."};

int dlpopup_rows(app_t *app)
{
    return (720 + app->extra_h - DLPOPUP_ROWS_Y) / DLPOPUP_STEP;
}

static void row_text(app_t *app, char const *title, int duration, float y)
{
    char cut[80 + 4];
    char time[32];

    utf8_cut(cut, title, 70);
    widget_text(app, cut, (float [3]){300, y + 8, 22}, &sfWhite);
    if (duration < 0)
        snprintf(time, sizeof(time), "?");
    else
        format_time(time, sizeof(time), duration * 1000L);
    widget_text(app, time, (float [3]){1070, y + 8, 22}, &gray);
}

static int hidden(app_t *app, int i)
{
    int first = app->dlscene->scroll;

    return i < first || i - first >= dlpopup_rows(app);
}

/*
** returns 1 if a decision was taken (the entry is then freed)
*/
static int flagged_buttons(app_t *app, dl_entry_t *e, float y)
{
    if (widget_button(app, (float [4]){1180, y, 190, 44}, "Télécharger")) {
        download_accept(app, e);
        return 1;
    }
    if (widget_button(app, (float [4]){1385, y, 130, 44}, "Refuser")) {
        download_refuse(app, e);
        return 1;
    }
    if (widget_button(app, (float [4]){1530, y, 125, 44}, "Exclure")) {
        download_exclude(app, e);
        return 1;
    }
    return 0;
}

/*
** the list changes after a decision: the loop stops there
*/
static void flagged_rows(app_t *app, download_t *dl)
{
    int i = -1;
    float y = DLPOPUP_ROWS_Y - DLPOPUP_STEP;
    char title[NAME_LEN + 32];

    for (dl_entry_t *e = dl->flagged; e; e = e->next) {
        if (!search_text(app, e->title))
            continue;
        i++;
        if (hidden(app, i))
            continue;
        y += DLPOPUP_STEP;
        snprintf(title, sizeof(title), "%s%s", e->was_refused ?
            "(refusé avant) " : "", e->title);
        row_text(app, title, e->duration, y);
        if (flagged_buttons(app, e, y))
            return;
    }
}

/*
** a tab is written "> name <" when it is shown
*/
static void tab(app_t *app, float const *xw, char const *name,
    dl_popup_t popup)
{
    dlscene_t *scene = app->dlscene;
    char label[64];

    snprintf(label, sizeof(label), scene->popup == popup ? "> %s <" : "%s",
        name);
    if (!widget_button(app, (float [4]){xw[0], 160, xw[1], 46}, label))
        return;
    scene->popup = popup;
    scene->scroll = 0;
    scene->confirm = 0;
    scene->archived = 0;
    scene->clear = 0;
    scene->forget = 0;
}

static void footer(app_t *app, dlscene_t *scene)
{
    float y = 790 + app->extra_h;

    if (scene->popup == DL_POPUP_FLAGGED && app->download.flagged) {
        if (widget_button(app, (float [4]){300, y, 300, 52},
                "Tout télécharger"))
            download_decide_all(app, 1);
        if (widget_button(app, (float [4]){620, y, 260, 52}, "Tout refuser"))
            download_decide_all(app, 0);
    }
    if (scene->popup == DL_POPUP_ABSENT)
        dlscene_absent_footer(app, y);
    if (scene->popup == DL_POPUP_UNAVAILABLE)
        dlscene_unavailable_footer(app, y);
    if (scene->popup == DL_POPUP_REFUSED || scene->popup == DL_POPUP_EXCLUDED)
        dlscene_archive_footer(app, y);
    if (widget_button(app, (float [4]){1455, y, 200, 52}, "Fermer"))
        scene->popup = DL_POPUP_NONE;
}

static void tabs(app_t *app)
{
    char label[64];

    snprintf(label, sizeof(label), "Hors durée (%d)",
        entries_count(app->download.flagged));
    tab(app, (float [2]){300, 195}, label, DL_POPUP_FLAGGED);
    snprintf(label, sizeof(label), "Refusés (%d)",
        rejected_count(app->lib, 0));
    tab(app, (float [2]){505, 175}, label, DL_POPUP_REFUSED);
    snprintf(label, sizeof(label), "Retirés (%d)",
        app->download.nb_absent);
    tab(app, (float [2]){690, 165}, label, DL_POPUP_ABSENT);
    snprintf(label, sizeof(label), "Exclus (%d)",
        excluded_count(app->lib, 0));
    tab(app, (float [2]){865, 160}, label, DL_POPUP_EXCLUDED);
    snprintf(label, sizeof(label), "Indisponibles (%d)",
        unavailable_count(app->lib));
    tab(app, (float [2]){1035, 235}, label, DL_POPUP_UNAVAILABLE);
    snprintf(label, sizeof(label), "À vérifier (%d)",
        dlscene_split_count(app));
    tab(app, (float [2]){1280, 225}, label, DL_POPUP_SPLITS);
    tab(app, (float [2]){1515, 140}, "Bilan", DL_POPUP_REPORT);
}

static void page(app_t *app, dl_popup_t popup)
{
    if (popup == DL_POPUP_FLAGGED)
        flagged_rows(app, &app->download);
    if (popup == DL_POPUP_REFUSED)
        dlscene_refused(app);
    if (popup == DL_POPUP_ABSENT)
        dlscene_absent(app);
    if (popup == DL_POPUP_EXCLUDED)
        dlscene_excluded(app);
    if (popup == DL_POPUP_REPORT)
        dlscene_report(app);
    if (popup == DL_POPUP_UNAVAILABLE)
        dlscene_unavailable(app);
    if (popup == DL_POPUP_SPLITS)
        dlscene_splits(app);
}

void dlscene_popup(app_t *app)
{
    sfFloatRect view = app->view_rect;
    float all[4] = {view.left, view.top, view.width, view.height};
    sfColor shade = {0, 0, 0, 170};
    sfColor back = {30, 30, 30, 245};

    widget_rect(app, all, &shade);
    widget_rect(app, (float [4]){260, 140, 1420, 730 + app->extra_h},
        &back);
    tabs(app);
    widget_text(app, help[app->dlscene->popup], (float [3]){300, 235, 22},
        &gray);
    if (app->search.changed) {
        app->search.changed = 0;
        app->dlscene->scroll = 0;
    }
    search_field(app, (float [4]){1200, 222, 455, 48});
    page(app, app->dlscene->popup);
    footer(app, app->dlscene);
}
