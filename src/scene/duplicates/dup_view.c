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
** delta = 0 only keeps the scroll valid (after a resize or a choice)
*/
void dupscene_scroll(app_t *app, float delta)
{
    dupscene_t *scene = app->dupscene;
    int max = dup_same_count(app) + scene->list.nb - 1;

    scene->scroll -= (int)delta;
    if (scene->scroll > max)
        scene->scroll = max;
    if (scene->scroll < 0)
        scene->scroll = 0;
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

void dupscene_view(app_t *app)
{
    dupscene_t *scene = app->dupscene;
    float bottom = CONTENT_BOTTOM + app->extra_h;
    int nb_same = dup_same_count(app);
    float y = DUP_ROWS_Y;
    float h = 0;

    header(app, scene);
    for (int n = scene->scroll; scene->ready
        && n < nb_same + scene->list.nb; n++) {
        h = item_h(app, n, nb_same);
        if (y > DUP_ROWS_Y && y + h > bottom)
            break;
        if (h > 0 && item(app, n, nb_same, y))
            break;
        y += h;
    }
    if (widget_button(app, (float [4]){130, 840 + app->extra_h, 260, 56},
            "Retour"))
        scene_back(app);
}
