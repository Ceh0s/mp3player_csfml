/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** main page of the edit screen: title, artists, aliases, buttons
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static void save(app_t *app)
{
    edit_t *edit = app->edit;

    song_set_title(edit->song, edit->title);
    song_set_artists(app->lib, edit->song, edit->artists, edit->nb_artists);
    library_changed(app);
    scene_back(app);
}

static void header(app_t *app, edit_t *edit)
{
    char info[NAME_LEN * 3];

    snprintf(info, sizeof(info), "Tag ID3 : %s - %s    |    Fichier : %s",
        edit->song->src_artists ? edit->song->src_artists : "?",
        edit->song->src_title ? edit->song->src_title : "?",
        edit->song->file ? edit->song->file : "?");
    widget_text(app, "Modifier la musique", (float [3]){130, 125, 40},
        &sfWhite);
    widget_text(app, info, (float [3]){130, 180, 22}, &gray);
    widget_text(app, "Titre", (float [3]){130, 228, 26}, &sfWhite);
    if (widget_field(app, (float [4]){130, 265, 1100, 56}, edit->title,
            edit->focus == FOCUS_TITLE))
        edit->focus = FOCUS_TITLE;
    edit_suggest_line(app, edit);
}

static void add_column(app_t *app, edit_t *edit)
{
    role_t role = edit->nb_artists > 0 ? ROLE_FEAT : ROLE_MAIN;

    widget_text(app, "Ajouter un artiste (Entrée pour valider)",
        (float [3]){1260, 400, 24}, &sfWhite);
    if (widget_field(app, (float [4]){1260, 440, 520, 56}, edit->input,
            edit->focus == FOCUS_ARTIST))
        edit->focus = FOCUS_ARTIST;
    for (int i = 0; i < edit->nb_suggest; i++) {
        if (widget_button(app, (float [4]){1260, 504 + i * 50, 520, 46},
                edit->suggest[i]->name)) {
            edit_add_artist(app, edit->suggest[i]->name, role);
            return;
        }
    }
}

static void aliases(app_t *app, edit_t *edit)
{
    char label[NAME_LEN + 16];
    int shown = 0;

    for (song_t *s = app->lib->songs; s && shown < 2; s = s->next) {
        if (s->merged_into != edit->song->id)
            continue;
        if (shown == 0)
            widget_text(app, "Fusionnées dans celle-ci :",
                (float [3]){1260, 770, 22}, &gray);
        snprintf(label, sizeof(label), "Séparer : %s", song_title(s));
        if (widget_button(app, (float [4]){1260, 805 + shown * 50, 520, 44},
                label)) {
            song_split(app, s);
            return;
        }
        shown++;
    }
}

/*
** image of the song, at the top of the right column
*/
static void image(app_t *app, edit_t *edit)
{
    rank_t rank = {edit->song, NULL, 0, 0, 0};

    widget_image(app, image_get(app, &rank, 0),
        (float [4]){1260, 228, 150, 150}, 0);
    edit->image.focused = edit->focus == FOCUS_IMAGE;
    if (image_box(app, &rank, &edit->image,
            (float [3]){1425, 228, 355}))
        edit->focus = FOCUS_IMAGE;
}

/*
** the song is deleted and its video never downloaded again; asked
** twice; impossible for a song that did not come from a video
*/
static void exclude_button(app_t *app, edit_t *edit, float y)
{
    float rect[4] = {1480, y, 300, 56};
    char id[VIDEO_ID];

    if (!song_video_id(app->lib, edit->song, id)) {
        widget_help(app, rect, "Ce titre ne vient pas d'une vidéo : "
            "rien à exclure des téléchargements");
        widget_button(app, rect, "(Exclure)");
        return;
    }
    widget_help(app, rect, "Supprime le fichier et ne télécharge plus "
        "jamais cette vidéo (stats gardées)");
    if (!widget_button(app, rect, edit->exclude ? "Confirmer" : "Exclure"))
        return;
    if (!edit->exclude) {
        edit->exclude = 1;
        return;
    }
    song_exclude(app, edit->song);
    scene_back(app);
}

static void buttons(app_t *app, edit_t *edit)
{
    float y = 840 + app->extra_h;

    if (widget_button(app, (float [4]){130, y, 260, 56}, "Enregistrer"))
        save(app);
    if (widget_button(app, (float [4]){410, y, 260, 56}, "Annuler"))
        scene_back(app);
    like_buttons(app, KIND_SONG, edit->song->id, (float [3]){1060, y + 6, 0});
    if (widget_button(app, (float [4]){690, y, 340, 56},
            "Fusionner avec...")) {
        edit->mode = EDIT_SEARCH;
        edit->focus = FOCUS_SEARCH;
        edit_refresh_lists(app);
    }
    exclude_button(app, edit, y);
}

void edit_main(app_t *app)
{
    edit_t *edit = app->edit;

    header(app, edit);
    image(app, edit);
    widget_text(app, "Artistes", (float [3]){130, 400, 26}, &sfWhite);
    for (int i = 0; i < edit->nb_artists; i++)
        edit_artist_row(app, edit, i);
    add_column(app, edit);
    aliases(app, edit);
    buttons(app, edit);
}
