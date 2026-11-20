/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** table of the scenes (same principle as st_update in wolf3d)
*/

#include "mp3player.h"

const scene_t scenes[NB_SCENES] = {
    {HOME, &home_init, &home_event, NULL, &home_draw,
        &home_layout, &home_destroy},
    {LIBRARY, &libscene_init, &libscene_event, &libscene_update,
        &libscene_draw, &libscene_layout, &libscene_destroy},
    {EDIT, &edit_init, &edit_event, NULL, &edit_draw, &edit_layout,
        &edit_destroy},
    {BATTLE, &battle_init, &battle_event, NULL, &battle_draw,
        &battle_layout, &battle_destroy},
    {RANKING, &ranking_init, &ranking_event, NULL, &ranking_draw,
        &ranking_layout, &ranking_destroy},
    {PLAYLIST, &pledit_init, &pledit_event, NULL, &pledit_draw,
        &pledit_layout, &pledit_destroy},
    {BOARDS, &boards_init, &boards_event, NULL, &boards_draw,
        &boards_layout, &boards_destroy},
    {DOWNLOAD, &dlscene_init, &dlscene_event, NULL, &dlscene_draw,
        &dlscene_layout, &dlscene_destroy}
};

/*
** Only the resources of the displayed scene are loaded:
** the old scene and its images are freed before the new one is created.
** next_scene is set by scene_open / scene_back (scene_nav.c).
*/
int scene_switch(app_t *app)
{
    if (app->next_scene == app->scene)
        return 0;
    scenes[app->scene].destroy(app);
    image_cache_clear(app);
    app->scene = app->next_scene;
    if (scenes[app->scene].init(app) == 84)
        return 84;
    scenes[app->scene].layout(app);
    if (scenes[app->scene].update)
        scenes[app->scene].update(app);
    return 0;
}
