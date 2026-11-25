/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** playlists receiving the downloaded songs (besides "Tous les titres"):
** one button per local playlist, chosen ones are written "> name <".
** The playlist shown in the library is chosen when coming from there.
*/

#include "mp3player.h"

/*
** when the screen opens from the library on a playlist
*/
void dlscene_preselect(app_t *app)
{
    dlscene_t *scene = app->dlscene;
    playlist_t *pl = playlist_by_id(app->lib, app->library_tab);

    if (app->path_len == 0 || app->path[app->path_len - 1] != LIBRARY
        || !pl || pl->auto_like != 0)
        return;
    scene->targets[0] = app->library_tab;
    scene->nb_targets = 1;
}

static int target_index(dlscene_t const *scene, int id)
{
    for (int i = 0; i < scene->nb_targets; i++)
        if (scene->targets[i] == id)
            return i;
    return -1;
}

static void toggle(dlscene_t *scene, int id)
{
    int i = target_index(scene, id);

    if (i >= 0) {
        scene->nb_targets--;
        scene->targets[i] = scene->targets[scene->nb_targets];
        return;
    }
    if (scene->nb_targets >= DL_TARGETS)
        return;
    scene->targets[scene->nb_targets] = id;
    scene->nb_targets++;
}

/*
** the buttons go on a second line when the first is full, those that
** do not fit on it are not shown
*/
static void target_button(app_t *app, float *pos, playlist_t const *pl)
{
    char label[NAME_LEN + 8];
    int chosen = target_index(app->dlscene, pl->id) >= 0;
    float width = 0;

    snprintf(label, sizeof(label), chosen ? "> %s <" : "%s", pl->name);
    width = widget_text_width(app, label, 22) + 28;
    if (pos[0] + width > 1780) {
        pos[0] = 400;
        pos[1] += 56;
    }
    if (pos[1] > pos[2])
        return;
    if (widget_button(app, (float [4]){pos[0], pos[1], width, 46}, label))
        toggle(app->dlscene, pl->id);
    pos[0] += width + 10;
}

static int has_manual(library_t *lib)
{
    for (playlist_t *pl = lib->playlists; pl; pl = pl->next)
        if (pl->auto_like == 0)
            return 1;
    return 0;
}

void dlscene_targets(app_t *app, float y)
{
    float pos[3] = {400, y, y + 56};

    widget_text(app, "Ajouter aussi à :", (float [3]){130, y + 10, 24},
        &sfWhite);
    if (!has_manual(app->lib))
        widget_text(app, "aucune playlist", (float [3]){400, y + 10, 22},
            &sfWhite);
    for (playlist_t *pl = app->lib->playlists; pl; pl = pl->next)
        if (pl->auto_like == 0)
            target_button(app, pos, pl);
}
