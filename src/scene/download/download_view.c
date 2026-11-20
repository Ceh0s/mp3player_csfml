/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the download screen: address field, button, progress and
** status of the download (called to draw it and to handle a click)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor green = {120, 220, 120, 255};
static const sfColor orange = {255, 160, 60, 255};

/*
** the field is emptied once the download started
*/
void dlscene_start(app_t *app)
{
    if (download_start(app, app->dlscene->url) == 0)
        app->dlscene->url[0] = '\0';
}

static void progress(app_t *app, download_t const *dl)
{
    sfColor back = {70, 70, 70, 255};
    sfColor full = {255, 200, 0, 255};
    char line[32];

    widget_rect(app, (float [4]){130, 420, 1650, 16}, &back);
    widget_rect(app, (float [4]){130, 420, 16.5 * dl->percent, 16}, &full);
    snprintf(line, sizeof(line), "%.1f %%", dl->percent);
    widget_text(app, line, (float [3]){130, 450, 26}, &sfWhite);
    widget_text(app, dl->last, (float [3]){130, 495, 20}, &gray);
    if (widget_button(app, (float [4]){1530, 330, 250, 56}, "Annuler"))
        download_cancel(app);
}

static void status(app_t *app, download_t const *dl)
{
    if (dl->state == DL_DONE)
        widget_text(app, "Terminé : la musique est dans la bibliothèque.",
            (float [3]){130, 420, 26}, &green);
    if (dl->state != DL_FAILED)
        return;
    widget_text(app, "Échec du téléchargement :",
        (float [3]){130, 420, 26}, &orange);
    widget_text(app, dl->last, (float [3]){130, 465, 20}, &gray);
}

void dlscene_view(app_t *app)
{
    download_t *dl = &app->download;

    widget_text(app, "Télécharger une musique", (float [3]){130, 115, 40},
        &sfWhite);
    widget_text(app, "Colle l'adresse d'une vidéo ou d'un son (YouTube, "
        "SoundCloud, Bandcamp...) : le mp3 est enregistré dans music/.",
        (float [3]){130, 180, 22}, &gray);
    widget_field(app, (float [4]){130, 240, 1650, 56}, app->dlscene->url, 1);
    if (dl->state == DL_RUNNING)
        progress(app, dl);
    else if (widget_button(app, (float [4]){130, 330, 300, 56},
            "Télécharger"))
        dlscene_start(app);
    status(app, dl);
    if (widget_button(app, (float [4]){130, 840 + app->extra_h, 260, 56},
            "Retour"))
        scene_back(app);
}
