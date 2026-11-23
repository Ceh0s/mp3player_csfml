/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** home screen: a big play button to open the library, and the menu
** of the other screens
*/

#include "mp3player.h"

enum home_ids {
    HOME_PLAY,
    HOME_CLOSE
};

static const ui_def_t home_ui[] = {
    {HOME_PLAY, ANCHOR_CENTER,
        {806, 386, 307, 307, 0, 0, ATLAS_CELL, ATLAS_CELL}},
    {HOME_CLOSE, ANCHOR_TOP,
        {24, 16, 64, 64, ATLAS_CELL * 5, 0, ATLAS_CELL, ATLAS_CELL}}
};

int home_init(app_t *app)
{
    app->home = calloc(1, sizeof(home_t));
    if (!app->home)
        return 84;
    app->home->bg = background_create(BG_PATH, &app->home->bg_texture);
    app->home->ui = ui_create(app->atlas, home_ui, 2);
    if (!app->home->bg || !app->home->ui)
        return 84;
    return 0;
}

/*
** drawn with the widgets, under the play button (centered anchor)
*/
static void menu(app_t *app)
{
    float y = 740 + app->extra_h / 2;

    if (widget_button(app, (float [4]){160, y, 250, 64}, "Bibliothèque"))
        scene_open(app, LIBRARY);
    if (widget_button(app, (float [4]){430, y, 250, 64}, "Battle"))
        scene_open(app, BATTLE);
    if (widget_button(app, (float [4]){700, y, 250, 64}, "Classements"))
        scene_open(app, RANKING);
    if (widget_button(app, (float [4]){970, y, 250, 64}, "Statistiques"))
        scene_open(app, STATS);
    if (widget_button(app, (float [4]){1240, y, 250, 64}, "Doublons")) {
        app->dup_since = 0;
        scene_open(app, DUPLICATES);
    }
    if (widget_button(app, (float [4]){1510, y, 250, 64}, "Paramètres"))
        scene_open(app, SETTINGS);
}

void home_event(app_t *app, sfEvent *event)
{
    int id = -1;

    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    id = ui_hit(app, app->home->ui);
    if (id == HOME_PLAY)
        scene_open(app, LIBRARY);
    if (id == HOME_CLOSE)
        sfRenderWindow_close(app->window);
    app->wg->mode = WG_CLICK;
    menu(app);
    app->wg->mode = WG_DRAW;
}

void home_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->home->bg, NULL);
    ui_draw(app, app->home->ui);
    app->wg->mode = WG_DRAW;
    menu(app);
}

void home_layout(app_t *app)
{
    background_layout(app, app->home->bg);
    ui_layout(app->home->ui, app->extra_h);
}

void home_destroy(app_t *app)
{
    if (!app->home)
        return;
    if (app->home->bg)
        sfSprite_destroy(app->home->bg);
    if (app->home->bg_texture)
        sfTexture_destroy(app->home->bg_texture);
    ui_destroy(app->home->ui);
    free(app->home);
    app->home = NULL;
}
