/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** duplicates screen: the songs present several times (uploads of the
** same song), found from their titles, artists and durations, then
** confirmed by their sound. One of them is kept and the others merged
** into it.
** Opened from the home screen (every song) or after a download (only
** the new songs, app->dup_since).
*/

#include "mp3player.h"

int dupscene_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};
    dupscene_t *scene = calloc(1, sizeof(dupscene_t));

    app->dupscene = scene;
    if (!scene)
        return 84;
    scene->bg = background_create(BG_BLUR_PATH, &scene->bg_texture);
    scene->panel = rect_create(panel, &dark);
    if (!scene->bg || !scene->panel)
        return 84;
    scene->list.since = app->dup_since;
    if (dup_find(app->lib, &scene->list) == 84)
        return 84;
    return dupscene_todo(app);
}

void dupscene_event(app_t *app, sfEvent *event)
{
    if (event->type == sfEvtMouseWheelScrolled)
        dupscene_scroll(app, event->mouseWheelScroll.delta);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    dupscene_view(app);
    app->wg->mode = WG_DRAW;
}

void dupscene_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->dupscene->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->dupscene->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    dupscene_view(app);
    widget_tip_draw(app);
}

void dupscene_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->dupscene->bg);
    sfRectangleShape_setSize(app->dupscene->panel, size);
    dupscene_scroll(app, 0);
}

/*
** the envelopes read here are saved once, when leaving
*/
void dupscene_destroy(app_t *app)
{
    dupscene_t *scene = app->dupscene;

    if (!scene)
        return;
    db_save_envelopes(app->lib);
    if (scene->bg)
        sfSprite_destroy(scene->bg);
    if (scene->bg_texture)
        sfTexture_destroy(scene->bg_texture);
    if (scene->panel)
        sfRectangleShape_destroy(scene->panel);
    preview_stop(app);
    free(scene->list.groups);
    free(scene->todo);
    free(scene);
    app->dupscene = NULL;
}
