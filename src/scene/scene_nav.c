/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** navigation between the scenes, like the path of a folder:
** app->path holds the parent scenes, from the home screen to the current
** one (excluded). scene_open goes down ("cd battle"), scene_back goes
** up ("cd .."): it closes the app when already on the home screen.
** A scene reachable from several places (battle, rankings) then goes
** back to the one it was opened from.
*/

#include "mp3player.h"

/*
** Opening a scene already in the path cuts the path there (like going
** to a parent folder): the path cannot loop, so NB_SCENES cases are
** always enough. Nothing is done if a change is already asked during
** this tick (two clicks or keys in the same tick).
*/
void scene_open(app_t *app, scene_id_t id)
{
    if (app->next_scene != app->scene || id == app->scene)
        return;
    for (int i = 0; i < app->path_len; i++) {
        if (app->path[i] == id) {
            app->path_len = i;
            app->next_scene = id;
            return;
        }
    }
    app->path[app->path_len] = app->scene;
    app->path_len++;
    app->next_scene = id;
}

void scene_back(app_t *app)
{
    if (app->next_scene != app->scene)
        return;
    if (app->path_len == 0) {
        sfRenderWindow_close(app->window);
        return;
    }
    app->path_len--;
    app->next_scene = app->path[app->path_len];
}
