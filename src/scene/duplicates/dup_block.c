/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** one group of the duplicates screen: how sure it is (likeness of the
** sound), then one line per song (dup_row.c) with "Garder celui-ci"
** (every other song is merged into it) and "Pas un doublon" (the song
** leaves the group, never proposed again with the others)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor green = {120, 220, 120, 255};
static const sfColor orange = {255, 160, 60, 255};

float dup_block_h(dup_group_t const *group)
{
    return DUP_HEAD_H + group->nb * DUP_ROW_H + 16;
}

static void verdict(app_t *app, dup_group_t const *group, float y)
{
    char line[128];
    sfColor const *color = &gray;
    int best = -1;

    for (int i = 1; i < group->nb; i++)
        best = group->audio[i] > best ? group->audio[i] : best;
    if (best >= DUP_SURE)
        snprintf(line, sizeof(line), "%d versions : même enregistrement",
            group->nb);
    else if (best >= 0)
        snprintf(line, sizeof(line), "%d versions : son proche, à écouter",
            group->nb);
    else
        snprintf(line, sizeof(line), "%d versions : son non comparé "
            "(fichier illisible ou trop long)", group->nb);
    if (best >= 0)
        color = best >= DUP_SURE ? &green : &orange;
    widget_text(app, line, (float [3]){130, y + 8, 24}, color);
}

/*
** the song i is not a duplicate of the others of the group
*/
static void not_duplicate(app_t *app, dup_group_t *group, int i)
{
    for (int j = 0; j < group->nb; j++)
        if (j != i)
            not_dup_add(app->lib, group->songs[i]->id, group->songs[j]->id);
    db_save_not_dups(app->lib);
    group->songs[i] = NULL;
}

/*
** 0 : nothing clicked, 1 : keep, 2 : not a duplicate, 3 : other click
*/
static int choice(app_t *app, dup_group_t *group, int i, float y)
{
    float no[4] = {1495, y + 8, 285, 44};

    if (dup_row(app, group, i, y))
        return 3;
    widget_help(app, no, "Ce titre ne sera plus jamais proposé avec les "
        "autres du groupe");
    if (widget_button(app, (float [4]){1215, y + 8, 265, 44},
            "Garder celui-ci"))
        return 1;
    if (widget_button(app, no, "Pas un doublon"))
        return 2;
    return 0;
}

/*
** returns 1 if a click was used (the groups may have changed)
*/
int dup_block(app_t *app, dup_group_t *group, float y)
{
    int chosen = 0;

    verdict(app, group, y);
    for (int i = 0; i < group->nb && chosen == 0; i++) {
        chosen = choice(app, group, i, y + DUP_HEAD_H + i * DUP_ROW_H);
        if (chosen == 1)
            dup_keep_all(app, group, i);
        if (chosen == 2)
            not_duplicate(app, group, i);
    }
    if (chosen == 1 || chosen == 2) {
        preview_stop(app);
        dupscene_clean(app);
    }
    return chosen != 0;
}
