/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** removal of the song from the edit screen, both asked twice (stats
** kept, the song becomes absent): "Exclure" (its video is never
** downloaded again, only for a song that came from a video) and
** "Supprimer du lecteur" (only its files)
*/

#include "mp3player.h"

/*
** labels = {first click, second click}; returns 1 once the second click
** confirms the action (edit->exclude holds the action asked)
*/
static int confirm(app_t *app, float const *rect, int action,
    char const *const *labels)
{
    edit_t *edit = app->edit;
    int asked = edit->exclude == action;

    if (!widget_button(app, rect, labels[asked]))
        return 0;
    edit->exclude = asked ? 0 : action;
    return asked;
}

static void exclude_button(app_t *app, edit_t *edit, float y)
{
    float rect[4] = {1480, y, 300, 56};
    char id[VIDEO_ID];

    if (!song_video_id(app->lib, edit->song, id))
        return;
    widget_help(app, rect, "Ne sera plus jamais téléchargée");
    if (!confirm(app, rect, EDIT_ASK_EXCLUDE,
            (char const *[2]){"Exclure", "Confirmer"}))
        return;
    song_exclude(app, edit->song);
    scene_back(app);
}

static void delete_button(app_t *app, edit_t *edit, float y)
{
    float rect[4] = {1380, y, 400, 56};
    char const *labels[2] = {"Supprimer du lecteur",
        "Confirmer la suppression"};

    widget_help(app, rect, "Les stats sont gardées");
    if (!confirm(app, rect, EDIT_ASK_DELETE, labels))
        return;
    song_delete_files(app, edit->song);
    scene_back(app);
}

void edit_remove_buttons(app_t *app, edit_t *edit, float y)
{
    exclude_button(app, edit, y);
    delete_button(app, edit, y - 72);
}
