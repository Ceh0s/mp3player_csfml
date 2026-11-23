/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page of the list over the download screen: the songs no longer in
** the playlist of the site. Each one is kept, taken out of the local
** playlist, or its file is removed (asked twice).
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static void row_text(app_t *app, song_t const *song, float y)
{
    char line[NAME_LEN * 2 + 8];
    char cut[90 + 4];

    snprintf(line, sizeof(line), "%s%s%s", song_title(song),
        song->artists ? " - " : "", song->artists ? song->artists : "");
    utf8_cut(cut, line, 90);
    widget_text(app, cut, (float [3]){300, y + 8, 22}, &sfWhite);
}

/*
** returns 1 if a button was clicked (the list changed)
*/
static int row_buttons(app_t *app, int i, int id, float y)
{
    dlscene_t *scene = app->dlscene;

    if (widget_button(app, (float [4]){1010, y, 140, 44}, "Garder")) {
        download_absent_keep(app, i);
        return 1;
    }
    if (widget_button(app, (float [4]){1165, y, 160, 44}, "Retirer")) {
        download_absent_unlist(app, i);
        return 1;
    }
    if (!widget_button(app, (float [4]){1340, y, 315, 44},
            scene->confirm == id ? "Confirmer ?" : "Supprimer le fichier"))
        return 0;
    if (scene->confirm != id) {
        scene->confirm = id;
        return 1;
    }
    scene->confirm = 0;
    download_absent_delete(app, i);
    return 1;
}

void dlscene_absent(app_t *app)
{
    download_t *dl = &app->download;
    int first = app->dlscene->scroll;
    song_t *song = NULL;
    float y = 0;

    for (int i = first; i < dl->nb_absent
        && i - first < dlpopup_rows(app); i++) {
        song = song_by_id(app->lib, dl->absent[i]);
        y = DLPOPUP_ROWS_Y + (i - first) * DLPOPUP_STEP;
        if (song)
            row_text(app, song, y);
        if (row_buttons(app, i, dl->absent[i], y))
            return;
    }
}

/*
** "Retirer" only takes the song out of the playlist; the file is only
** removed by its own button, one song at a time
*/
void dlscene_absent_footer(app_t *app, float y)
{
    download_t *dl = &app->download;

    if (dl->nb_absent == 0)
        return;
    if (widget_button(app, (float [4]){300, y, 260, 52}, "Tout garder"))
        while (dl->nb_absent > 0)
            download_absent_keep(app, 0);
    if (widget_button(app, (float [4]){580, y, 300, 52}, "Tout retirer"))
        while (dl->nb_absent > 0)
            download_absent_unlist(app, 0);
    widget_text(app, "Retirer = de la playlist seulement, le fichier reste",
        (float [3]){900, y + 14, 20}, &gray);
}
