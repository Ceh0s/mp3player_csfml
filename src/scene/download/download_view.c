/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the download screen: field of the addresses (separated by
** ;), buttons and options, target playlists, progress of the download
** (called to draw it and to handle a click); the list of the videos put
** aside is shown over it when open
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
/*
** the field is emptied once the download started
*/
void dlscene_start(app_t *app)
{
    dl_options_t opt;

    dlscene_options(app, &opt);
    if (download_start(app, app->dlscene->url, &opt) == 0)
        app->dlscene->url[0] = '\0';
}

/*
** number of links typed, cut as download_start will cut them
*/
static int count_links(char const *text)
{
    int count = 0;
    int in_link = 0;

    for (int i = 0; text[i]; i++) {
        if (strchr("; \t", text[i])) {
            in_link = 0;
            continue;
        }
        count += in_link == 0;
        in_link = 1;
    }
    return count;
}

static void progress(app_t *app, download_t const *dl)
{
    sfColor back = {70, 70, 70, 255};
    sfColor full = {255, 200, 0, 255};
    char line[128];
    int len = 0;

    widget_rect(app, (float [4]){130, 600, 1650, 16}, &back);
    widget_rect(app, (float [4]){130, 600, 16.5 * dl->percent, 16}, &full);
    if (dl->step == DL_STEP_LIST)
        len = snprintf(line, sizeof(line), "Lecture de la liste");
    else
        len = snprintf(line, sizeof(line), "%.1f %%", dl->percent);
    if (dl->total > 1)
        len += snprintf(line + len, sizeof(line) - len, "     lien %d / %d",
            dl->next, dl->total);
    if (dl->items > 0 && dl->step == DL_STEP_GET)
        snprintf(line + len, sizeof(line) - len, "     élément %d / %d",
            dl->item, dl->items);
    widget_text(app, line, (float [3]){130, 630, 26}, &sfWhite);
    dlscene_stats(app, dl);
    widget_text(app, dl->last, (float [3]){130, 760, 20}, &gray);
}

/*
** title, help and field with the number of links typed
*/
static void header(app_t *app)
{
    dlscene_t *scene = app->dlscene;
    char count[32];

    widget_text(app, "Télécharger des musiques", (float [3]){130, 115, 40},
        &sfWhite);
    widget_text(app, "Adresses, séparées par ;", (float [3]){130, 195, 22},
        &gray);
    if (widget_field(app, (float [4]){130, 240, 1650, 56},
            scene->url, scene->focus == DL_FIELD_URL))
        scene->focus = DL_FIELD_URL;
    snprintf(count, sizeof(count), "%d lien(s)", count_links(scene->url));
    widget_text(app, count, (float [3]){1600, 305, 20}, &gray);
}

/*
** "Annuler" takes the place of "Télécharger" while a queue runs; the
** cookies of the browser are only needed for a private playlist
*/
static void buttons(app_t *app, download_t *dl)
{
    float cookies[4] = {460, 330, 420, 56};

    if (dl->queue && widget_button(app, (float [4]){130, 330, 300, 56},
            "Annuler"))
        download_cancel(app);
    else if (!dl->queue && widget_button(app,
            (float [4]){130, 330, 300, 56}, "Télécharger"))
        dlscene_start(app);
    if (widget_button(app, cookies, dl->cookies ? "Cookies " DL_BROWSER
            " : oui" : "Cookies " DL_BROWSER " : non"))
        dl->cookies = !dl->cookies;
    widget_help(app, cookies, "Pour une playlist privée");
    dlscene_durations(app, 330);
}

/*
** opens the list of the videos put aside (and of those refused); it
** also opens alone, once, when a queue ended or was stopped: on the
** report, or on the songs absent from the playlist of the site
*/
static void flagged_button(app_t *app, download_t *dl)
{
    char label[64];
    dl_popup_t page = DL_POPUP_FLAGGED;

    snprintf(label, sizeof(label), "Hors durée : %d",
        entries_count(dl->flagged));
    if (dl->ask || dl->show_report)
        page = dl->nb_absent > 0 ? DL_POPUP_ABSENT : DL_POPUP_REPORT;
    if (!widget_button(app, (float [4]){1480, 330, 300, 56}, label)
        && !dl->ask && !dl->show_report)
        return;
    dl->ask = 0;
    dl->show_report = 0;
    app->dlscene->popup = page;
    app->dlscene->scroll = 0;
}

void dlscene_view(app_t *app)
{
    download_t *dl = &app->download;

    if (app->dlscene->popup != DL_POPUP_NONE) {
        dlscene_popup(app);
        return;
    }
    header(app);
    buttons(app, dl);
    flagged_button(app, dl);
    dlscene_targets(app, 410);
    dlscene_link(app, 530);
    if (dl->state == DL_RUNNING) {
        progress(app, dl);
        dlscene_failures(app, dl, 795);
    }
    dlscene_status(app, dl);
    if (widget_button(app, (float [4]){130, 840 + app->extra_h, 260, 56},
            "Retour"))
        scene_back(app);
}
