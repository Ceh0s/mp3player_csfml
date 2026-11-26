/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the duplicates screen (called to draw it and to handle a
** click): progress of the reading of the sound, then the files of
** identical sound and one block per group (dup_block.c), filtered by
** the search field
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

/*
** a group is shown if one of its songs matches the search
*/
static int shown(app_t *app, dup_group_t const *group)
{
    for (int i = 0; i < group->nb; i++)
        if (search_song(app, group->songs[i]))
            return 1;
    return 0;
}

/*
** the wheel moves about two songs
*/
void dupscene_scroll(app_t *app, float delta)
{
    scroll_by(&app->dupscene->rows, -delta * 2 * DUP_ROW_H);
}

static void status(app_t *app, dupscene_t *scene)
{
    char line[160];
    char const *since = scene->list.since ? " parmi les nouveaux titres" : "";

    if (!scene->ready)
        snprintf(line, sizeof(line), "Lecture du son : %d / %d...",
            scene->done, scene->nb_todo);
    else if (scene->list.nb == 0 && !app->lib->same_files)
        snprintf(line, sizeof(line), "Aucun doublon trouvé%s.", since);
    else
        snprintf(line, sizeof(line), "%d groupe(s)%s", scene->list.nb,
            since);
    widget_text(app, line, (float [3]){130, 180, 22}, &gray);
    if (scene->auto_merged == 0)
        return;
    snprintf(line, sizeof(line), "%d fusion(s) automatique(s)",
        scene->auto_merged);
    widget_text(app, line, (float [3]){130, 210, 22}, &gray);
}

static void header(app_t *app, dupscene_t *scene)
{
    float remove[4] = {1260, 115, 520, 52};
    settings_t *set = &app->lib->settings;

    widget_text(app, "Doublons", (float [3]){130, 115, 40}, &sfWhite);
    status(app, scene);
    search_field(app, (float [4]){700, 115, 520, 52});
    if (widget_button(app, remove, set->dup_remove ? "Supprimer le fichier "
            "en trop : oui" : "Supprimer le fichier en trop : non")) {
        set->dup_remove = !set->dup_remove;
        db_save_settings(app->lib);
    }
    widget_help(app, remove, "Aussi pour les fusions automatiques");
}

/*
** height of the item n: a file of identical sound, or a group (0 if the
** search leaves it out)
*/
static float item_h(app_t *app, int n, int nb_same)
{
    dup_group_t *group = NULL;

    if (n < nb_same)
        return DUP_SAME_H;
    group = &app->dupscene->list.groups[n - nb_same];
    return shown(app, group) ? dup_block_h(group) : 0;
}

/*
** returns 1 if the list changed
*/
static int item(app_t *app, int n, int nb_same, float y)
{
    if (n < nb_same)
        return dup_same_row(app, n, y);
    return dup_block(app, &app->dupscene->list.groups[n - nb_same], y);
}

/*
** the items go from DUP_ROWS_Y to the bottom of the table
*/
static void frame(app_t *app, dupscene_t *scene, int nb_same)
{
    float area[4] = {PANEL_X, DUP_ROWS_Y, PANEL_W,
        CONTENT_BOTTOM + app->extra_h - DUP_ROWS_Y};
    float content = 0;

    for (int n = 0; scene->ready && n < nb_same + scene->list.nb; n++)
        content += item_h(app, n, nb_same);
    scroll_frame(&scene->rows, area, content);
}

/*
** offset : height of the items above, from the top of the list
*/
static void items(app_t *app, dupscene_t *scene, int nb_same)
{
    float offset = 0;
    float y = 0;
    float h = 0;

    if (!scroll_rows_begin(app, &scene->rows))
        return;
    for (int n = 0; scene->ready && n < nb_same + scene->list.nb; n++) {
        h = item_h(app, n, nb_same);
        y = scroll_y(&scene->rows, offset);
        offset += h;
        if (h > 0 && scroll_seen(&scene->rows, y, h)
            && item(app, n, nb_same, y))
            break;
    }
    scroll_rows_end(app);
}

void dupscene_view(app_t *app)
{
    dupscene_t *scene = app->dupscene;
    int nb_same = dup_same_count(app);

    header(app, scene);
    frame(app, scene, nb_same);
    items(app, scene, nb_same);
    scroll_bar_draw(app, &scene->rows);
    if (widget_back(app))
        scene_back(app);
}
