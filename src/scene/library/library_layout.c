/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** placement of the library screen depending on the screen height,
** and its destruction
*/

#include "mp3player.h"

static void destroy_shapes(libscene_t *scene)
{
    sfRectangleShape *shapes[4] = {scene->panel, scene->bar,
        scene->progress_bg, scene->progress};
    sfText *texts[4] = {scene->now_title, scene->now_artist, scene->time,
        scene->hint};

    for (int i = 0; i < 4; i++)
        if (shapes[i])
            sfRectangleShape_destroy(shapes[i]);
    for (int i = 0; i < 4; i++)
        if (texts[i])
            sfText_destroy(texts[i]);
    for (int i = 0; scene->titles && scene->titles[i]; i++)
        sfText_destroy(scene->titles[i]);
    free(scene->titles);
}

void libscene_layout(app_t *app)
{
    libscene_t *scene = app->libscene;
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, scene->bg);
    ui_layout(scene->ui, app->extra_h);
    sfRectangleShape_setSize(scene->panel, size);
    scene->rows = (size.y - 2 * (LIST_Y - 110)) / LIST_STEP;
    list_scroll(app, 0);
    bar_layout(app);
}

void libscene_destroy(app_t *app)
{
    if (!app->libscene)
        return;
    destroy_shapes(app->libscene);
    if (app->libscene->bg)
        sfSprite_destroy(app->libscene->bg);
    if (app->libscene->bg_texture)
        sfTexture_destroy(app->libscene->bg_texture);
    ui_destroy(app->libscene->ui);
    free(app->libscene);
    app->libscene = NULL;
}
