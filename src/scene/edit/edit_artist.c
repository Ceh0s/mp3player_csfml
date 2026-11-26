/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** artist page opened from the edit screen (the edits in progress are
** kept, since the edit scene stays loaded) or from the rankings, with
** the choice of the image of the artist (round, like in YT Music) and
** its new name (artist_rename.c: a name already taken merges them)
*/

#include "mp3player.h"
#include <strings.h>

static void image(app_t *app, edit_t *edit)
{
    rank_t rank = {NULL, edit->artist, 0, 0, 0};

    widget_image(app, image_get(app, &rank, 0),
        (float [4]){1430, 130, 260, 260}, 1);
    edit->image.focused = edit->focus == FOCUS_IMAGE;
    if (image_box(app, &rank, &edit->image,
            (float [3]){1300, 420, 480}))
        edit->focus = FOCUS_IMAGE;
}

/*
** the artists of the song being edited follow the new name
*/
static void follow_name(edit_t *edit, char const *old, char const *name)
{
    for (int i = 0; i < edit->nb_artists; i++)
        if (strcasecmp(edit->artists[i].name, old) == 0)
            snprintf(edit->artists[i].name, NAME_LEN, "%s", name);
}

void edit_rename_artist(app_t *app)
{
    edit_t *edit = app->edit;
    char old[NAME_LEN];
    artist_t *result = NULL;

    if (!edit->artist)
        return;
    snprintf(old, NAME_LEN, "%s", edit->artist->name);
    result = artist_rename(app, edit->artist, edit->rename);
    if (!result)
        return;
    edit->artist = result;
    edit->renamed = result;
    snprintf(edit->rename, NAME_LEN, "%s", result->name);
    follow_name(edit, old, result->name);
}

static void rename_box(app_t *app, edit_t *edit)
{
    float field[4] = {1300, 640, 330, 56};
    float button[4] = {1650, 640, 130, 56};

    if (edit->renamed != edit->artist) {
        snprintf(edit->rename, NAME_LEN, "%s", edit->artist->name);
        edit->renamed = edit->artist;
    }
    widget_text(app, "Nom de l'artiste", (float [3]){1300, 600, 22},
        &sfWhite);
    if (widget_field(app, field, edit->rename, edit->focus == FOCUS_RENAME))
        edit->focus = FOCUS_RENAME;
    widget_help(app, button, "Un nom déjà pris réunit les deux artistes");
    if (widget_button(app, button, "Renommer"))
        edit_rename_artist(app);
}

void edit_artist(app_t *app)
{
    int back = 0;

    artist_page(app, app->edit->artist, &app->edit->titles, &back);
    if (app->edit->artist) {
        image(app, app->edit);
        rename_box(app, app->edit);
    }
    if (!back)
        return;
    if (!app->edit->song) {
        scene_back(app);
        return;
    }
    app->edit->mode = EDIT_MAIN;
    app->edit->focus = FOCUS_TITLE;
    app->edit->image.msg[0] = '\0';
}
