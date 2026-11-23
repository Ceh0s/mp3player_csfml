/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** saves the stats then frees everything
*/

#include "mp3player.h"

static void destroy_shared(app_t *app)
{
    if (app->font)
        sfFont_destroy(app->font);
    if (app->atlas)
        sfTexture_destroy(app->atlas);
    if (app->no_cover)
        sfTexture_destroy(app->no_cover);
    if (app->ticks)
        sfClock_destroy(app->ticks);
    if (app->delta)
        sfClock_destroy(app->delta);
    if (app->view)
        sfView_destroy(app->view);
    if (app->window)
        sfRenderWindow_destroy(app->window);
}

/*
** yt-dlp is stopped first, then what the download kept is freed
*/
static void destroy_download(app_t *app)
{
    download_cancel(app);
    entries_free(app->download.flagged);
    report_free(&app->download);
    free(app->download.absent);
    download_order_free(&app->download);
}

int app_destroy(app_t *app, int rtn_value)
{
    if (!app)
        return rtn_value;
    scenes[app->scene].destroy(app);
    destroy_download(app);
    if (app->player && app->lib)
        player_stop(app, END_QUIT);
    if (app->lib && db_save_all(app->lib) == 84)
        rtn_value = 84;
    library_destroy(app->lib);
    if (app->player)
        free(app->player->queue);
    free(app->player);
    widgets_destroy(app->wg);
    image_cache_clear(app);
    destroy_shared(app);
    free(app);
    return rtn_value;
}
