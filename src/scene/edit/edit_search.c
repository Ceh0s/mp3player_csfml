/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** merge pages of the edit screen: choice of the target, confirmation
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor orange = {255, 160, 60, 255};
static const sfColor green = {120, 220, 120, 255};

static int result_row(app_t *app, edit_t *edit, int i)
{
    song_t *song = edit->results[i];
    float y = 296 + i * 52;

    if (!widget_button(app, (float [4]){130, y, 1500, 48},
            song->present ? "" : "Fichier absent")) {
        widget_song(app, song, (float [4]){360, y + 3, 1250, 42},
            &sfWhite);
        return 0;
    }
    edit->target = song;
    edit->mode = EDIT_CONFIRM;
    return 1;
}

void edit_search(app_t *app)
{
    edit_t *edit = app->edit;
    char head[NAME_LEN + 64];

    snprintf(head, sizeof(head), "Fusionner « %s » avec...",
        song_title(edit->song));
    widget_text(app, head, (float [3]){130, 125, 36}, &sfWhite);
    widget_text(app, "Choisis la musique qui recevra les statistiques.",
        (float [3]){130, 178, 22}, &gray);
    widget_field(app, (float [4]){130, 220, 1100, 56}, edit->search, 1);
    for (int i = 0; i < edit->nb_results; i++)
        if (result_row(app, edit, i))
            return;
    if (widget_button(app, (float [4]){130, 840 + app->extra_h, 260, 56},
            "Retour")) {
        edit->mode = EDIT_MAIN;
        edit->focus = FOCUS_TITLE;
    }
}

static void song_column(app_t *app, song_t *song, float x, char const *head)
{
    char line[NAME_LEN + 64];
    char time[16];

    widget_text(app, head, (float [3]){x, 200, 24}, &gray);
    widget_text(app, song_title(song), (float [3]){x, 240, 30}, &sfWhite);
    widget_text(app, song->artists ? song->artists : "Artiste inconnu",
        (float [3]){x, 285, 24}, &sfWhite);
    format_time(time, sizeof(time), song->duration_ms);
    snprintf(line, sizeof(line), "Durée : %s", time);
    widget_text(app, line, (float [3]){x, 330, 22}, &gray);
    format_time(time, sizeof(time), song_total_listen(app->lib, song));
    snprintf(line, sizeof(line), "Temps d'écoute : %s", time);
    widget_text(app, line, (float [3]){x, 365, 22}, &gray);
    if (!song->present)
        widget_text(app, "Fichier absent", (float [3]){x, 400, 22}, &gray);
}

static void warnings(app_t *app, edit_t *edit)
{
    char warn[EDIT_WARNINGS][128];
    int nb = merge_warnings(edit->song, edit->target, warn);

    if (nb == 0)
        widget_text(app, "Aucune incohérence",
            (float [3]){130, 470, 24}, &green);
    for (int i = 0; i < nb; i++)
        widget_text(app, warn[i], (float [3]){130, 470 + i * 36, 24},
            &orange);
}

void edit_confirm(app_t *app)
{
    edit_t *edit = app->edit;
    float y = 840 + app->extra_h;

    widget_text(app, "Confirmer la fusion", (float [3]){130, 125, 40},
        &sfWhite);
    song_column(app, edit->song, 130, "Fusionnée");
    song_column(app, edit->target, 980, "Gardée");
    warnings(app, edit);
    if (widget_button(app, (float [4]){130, y, 360, 56},
            "Confirmer la fusion")) {
        song_merge(app, edit->song, edit->target);
        scene_back(app);
        return;
    }
    if (widget_button(app, (float [4]){510, y, 260, 56}, "Annuler"))
        edit->mode = EDIT_SEARCH;
}
