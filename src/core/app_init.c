/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** creates the app and the resources shared by every scene
*/

#include "mp3player.h"

/*
** the textures are smoothed, as they are drawn scaled
*/
static int load_textures(app_t *app)
{
    app->atlas = sfTexture_createFromFile(ATLAS_PATH, NULL);
    app->no_cover = sfTexture_createFromFile(NO_COVER_PATH, NULL);
    app->no_cover_dark = dark_invert(NO_COVER_PATH);
    if (!app->atlas || !app->no_cover || !app->no_cover_dark)
        return 84;
    sfTexture_setSmooth(app->atlas, sfTrue);
    sfTexture_setSmooth(app->no_cover, sfTrue);
    sfTexture_setSmooth(app->no_cover_dark, sfTrue);
    return 0;
}

static int load_shared(app_t *app)
{
    app->font = sfFont_createFromFile(FONT_PATH);
    app->fallback[0] = sfFont_createFromFile(FONT_JP_PATH);
    app->fallback[1] = sfFont_createFromFile(FONT_SERIF_PATH);
    app->run_text = sfText_create();
    app->ticks = sfClock_create();
    app->delta = sfClock_create();
    app->player = calloc(1, sizeof(player_t));
    app->lib = library_load();
    if (!app->font || !app->ticks || !app->delta || !app->player
        || !app->lib || load_textures(app) == 84)
        return 84;
    app->wg = widgets_create(app);
    if (!app->wg)
        return 84;
    srand(time(NULL));
    app->player->index = -1;
    app->show_images = 1;
    app->dl_range[0] = -1;
    app->dl_range[1] = -1;
    return 0;
}

app_t *app_create(void)
{
    app_t *app = NULL;

    if (signals_block() != 0)
        fprintf(stderr, "Cannot catch Ctrl+C: stats may be lost\n");
    app = calloc(1, sizeof(app_t));
    if (!app)
        return NULL;
    app->fullscreen = 1;
    app->tick_loop = TICK_MS;
    app->scene = HOME;
    app->next_scene = HOME;
    if (window_create(app) == 84 || view_create(app) == 84
        || load_shared(app) == 84 || scenes[app->scene].init(app) == 84) {
        fprintf(stderr, "Initialisation failed\n");
        app_destroy(app, 84);
        return NULL;
    }
    scenes[app->scene].layout(app);
    app->relayout = 0;
    return app;
}
