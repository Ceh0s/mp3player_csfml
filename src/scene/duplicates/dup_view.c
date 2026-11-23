/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the duplicates screen (called to draw it and to handle a
** click): progress of the reading of the sound, then one block per
** pair: how sure it is, the two songs, and the choices
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

int dupscene_rows(app_t *app)
{
    int rows = (CONTENT_BOTTOM + app->extra_h - DUP_ROWS_Y) / DUP_BLOCK_H;

    return rows > 0 ? rows : 1;
}

/*
** delta = 0 only keeps the scroll valid (after a resize or a choice)
*/
void dupscene_scroll(app_t *app, float delta)
{
    dupscene_t *scene = app->dupscene;
    int max = scene->list.nb - dupscene_rows(app);

    scene->scroll -= (int)delta;
    if (scene->scroll > max)
        scene->scroll = max;
    if (scene->scroll < 0)
        scene->scroll = 0;
}

static void status(app_t *app, dupscene_t *scene)
{
    char line[128];

    if (!scene->ready)
        snprintf(line, sizeof(line), "Lecture du son : %d / %d...",
            scene->done, scene->nb_todo);
    else if (scene->list.nb == 0)
        snprintf(line, sizeof(line), "Aucun doublon trouvé%s.",
            scene->list.since ? " parmi les nouveaux titres" : "");
    else
        snprintf(line, sizeof(line), "%d doublon(s) possible(s)%s : garde "
            "l'un des deux, l'autre y est fusionné (« Séparer » dans "
            "l'édition annule).", scene->list.nb, scene->list.since ?
            " parmi les nouveaux titres" : "");
    widget_text(app, line, (float [3]){130, 180, 22}, &gray);
}

static void header(app_t *app, dupscene_t *scene)
{
    float remove[4] = {1260, 115, 520, 52};

    widget_text(app, "Doublons", (float [3]){130, 115, 40}, &sfWhite);
    status(app, scene);
    if (widget_button(app, remove, scene->remove ? "Supprimer le fichier "
            "en trop : oui" : "Supprimer le fichier en trop : non"))
        scene->remove = !scene->remove;
    widget_help(app, remove, "Le fichier de la musique fusionnée est effacé "
        "de music/ (ses statistiques restent)");
}

void dupscene_view(app_t *app)
{
    dupscene_t *scene = app->dupscene;
    int rows = dupscene_rows(app);

    header(app, scene);
    for (int i = scene->scroll; scene->ready && i < scene->list.nb
        && i < scene->scroll + rows; i++)
        if (dup_block(app, i, DUP_ROWS_Y + (i - scene->scroll)
                * DUP_BLOCK_H))
            break;
    if (widget_button(app, (float [4]){130, 840 + app->extra_h, 260, 56},
            "Retour"))
        scene_back(app);
}
