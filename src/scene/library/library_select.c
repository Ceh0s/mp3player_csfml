/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** selection mode of the library: the songs checked (by their id, kept
** when the tab changes, lost when the screen is left), a click checks
** or unchecks a song, Shift + click checks every song up to the last
** one clicked
*/

#include "mp3player.h"

int select_has(selection_t const *sel, int id)
{
    for (int i = 0; i < sel->nb; i++)
        if (sel->ids[i] == id)
            return 1;
    return 0;
}

/*
** the song of the i-th id of the selection, NULL if it is not there
** any more
*/
song_t *select_song(app_t *app, int i)
{
    song_t *raw = song_by_id(app->lib, app->libscene->sel.ids[i]);
    song_t *song = song_resolve(app->lib, raw);

    return song && song->present ? song : NULL;
}

/*
** checked = 1 adds the song (if not there), 0 removes it
*/
static void select_set(selection_t *sel, int id, int checked)
{
    int *bigger = NULL;
    int kept = 0;

    if (checked && !select_has(sel, id)) {
        bigger = realloc(sel->ids, sizeof(int) * (sel->nb + 1));
        if (!bigger)
            return;
        sel->ids = bigger;
        sel->ids[sel->nb] = id;
        sel->nb++;
        return;
    }
    for (int i = 0; checked == 0 && i < sel->nb; i++)
        if (sel->ids[i] != id) {
            sel->ids[kept] = sel->ids[i];
            kept++;
        }
    if (checked == 0)
        sel->nb = kept;
}

/*
** row of the list under the mouse, anywhere on its line (the box on
** the left included), -1 if none
*/
int select_row(app_t *app)
{
    if (app->mouse.x < PANEL_X || app->mouse.x > PANEL_X + PANEL_W)
        return -1;
    return list_row_at(app);
}

void select_click(app_t *app, int row)
{
    libscene_t *scene = app->libscene;
    selection_t *sel = &scene->sel;
    int shift = sfKeyboard_isKeyPressed(sfKeyLShift)
        || sfKeyboard_isKeyPressed(sfKeyRShift);
    int from = sel->last < row ? sel->last : row;
    int to = sel->last < row ? row : sel->last;

    if (!shift || sel->last < 0 || sel->last >= scene->nb_view) {
        select_set(sel, scene->view[row]->id,
            !select_has(sel, scene->view[row]->id));
        sel->last = row;
        return;
    }
    for (int i = from; i <= to; i++)
        select_set(sel, scene->view[i]->id, 1);
    sel->last = row;
}

/*
** all = 1 checks every song of the tab shown, 0 unchecks everything
*/
void select_all(app_t *app, int all)
{
    libscene_t *scene = app->libscene;

    if (!all) {
        free(scene->sel.ids);
        scene->sel.ids = NULL;
        scene->sel.nb = 0;
        scene->sel.last = -1;
        return;
    }
    for (int i = 0; i < scene->nb_view; i++)
        select_set(&scene->sel, scene->view[i]->id, 1);
}
