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
    app->next_scene = LIBRARY;
}

static void header(app_t *app, edit_t *edit)
{
    char info[NAME_LEN * 3];
    char label[NAME_LEN * 2];

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
    if (!edit->has_proposal)
        return;
    snprintf(label, sizeof(label), "Suggestion : %s / %s  (cliquer pour "
        "appliquer)", edit->proposal_title, edit->proposal[0].name);
    if (widget_button(app, (float [4]){130, 335, 1100, 48}, label))
        edit_apply_proposal(app);
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

static void buttons(app_t *app, edit_t *edit)
{
    float y = 840 + app->extra_h;

    if (widget_button(app, (float [4]){130, y, 260, 56}, "Enregistrer"))
        save(app);
    if (widget_button(app, (float [4]){410, y, 260, 56}, "Annuler"))
        app->next_scene = LIBRARY;
    if (widget_button(app, (float [4]){690, y, 340, 56},
            "Fusionner avec...")) {
        edit->mode = EDIT_SEARCH;
        edit->focus = FOCUS_SEARCH;
        edit_refresh_lists(app);
    }
}

void edit_main(app_t *app)
{
    edit_t *edit = app->edit;

    header(app, edit);
    widget_text(app, "Artistes", (float [3]){130, 400, 26}, &sfWhite);
    for (int i = 0; i < edit->nb_artists; i++)
        edit_artist_row(app, edit, i);
    add_column(app, edit);
    aliases(app, edit);
    buttons(app, edit);
}
