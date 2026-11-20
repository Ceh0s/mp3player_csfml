/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** library screen: list of the songs and the player bar
*/

#include "mp3player.h"

enum lib_ids {
    LIB_CLOSE,
    LIB_BACK,
    LIB_PLAY,
    LIB_PAUSE,
    LIB_PREV,
    LIB_NEXT
};

static const ui_def_t lib_ui[] = {
    {LIB_CLOSE, ANCHOR_TOP,
        {24, 16, 64, 64, ATLAS_CELL * 5, 0, ATLAS_CELL, ATLAS_CELL}},
    {LIB_BACK, ANCHOR_TOP,
        {104, 16, 64, 64, ATLAS_CELL * 4, 0, ATLAS_CELL, ATLAS_CELL}},
    {LIB_PLAY, ANCHOR_BOTTOM,
        {912, 966, 96, 96, 0, 0, ATLAS_CELL, ATLAS_CELL}},
    {LIB_PAUSE, ANCHOR_BOTTOM,
        {912, 966, 96, 96, ATLAS_CELL, 0, ATLAS_CELL, ATLAS_CELL}},
    {LIB_PREV, ANCHOR_BOTTOM,
        {786, 974, 80, 80, ATLAS_CELL * 3, 0, ATLAS_CELL, ATLAS_CELL}},
    {LIB_NEXT, ANCHOR_BOTTOM,
        {1054, 974, 80, 80, ATLAS_CELL * 2, 0, ATLAS_CELL, ATLAS_CELL}}
};

int libscene_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor white = {255, 255, 255, 60};

    app->libscene = calloc(1, sizeof(libscene_t));
    if (!app->libscene)
        return 84;
    app->libscene->playlist_id = app->library_tab;
    app->libscene->bg = background_create(BG_BLUR_PATH,
        &app->libscene->bg_texture);
    app->libscene->ui = ui_create(app->atlas, lib_ui, 6);
    app->libscene->panel = rect_create(panel, &white);
    if (!app->libscene->bg || !app->libscene->ui || !app->libscene->panel
        || list_create(app) == 84 || bar_create(app) == 84)
        return 84;
    return 0;
}

static void click(app_t *app)
{
    int id = ui_hit(app, app->libscene->ui);
    int row = list_click(app);

    if (id == LIB_CLOSE)
        sfRenderWindow_close(app->window);
    if (id == LIB_BACK)
        scene_back(app);
    if (id == LIB_PLAY || id == LIB_PAUSE)
        player_toggle(app);
    if (id == LIB_PREV || id == LIB_NEXT)
        player_skip(app, id == LIB_PREV ? -1 : 1);
    if (row == -1 || id != -1)
        return;
    player_queue(app, app->libscene->playlist_id);
    if (app->libscene->view[row] == app->player->song)
        player_toggle(app);
    else
        player_play(app, row);
}

/*
** shortcuts to the battle and rankings screens (widgets)
*/
static void shortcuts(app_t *app)
{
    if (widget_button(app, (float [4]){1060, 20, 250, 56}, "Télécharger"))
        scene_open(app, DOWNLOAD);
    if (widget_button(app, (float [4]){1330, 20, 230, 56}, "Battle"))
        scene_open(app, BATTLE);
    if (widget_button(app, (float [4]){1580, 20, 250, 56}, "Classements"))
        scene_open(app, RANKING);
}

static void right_click(app_t *app)
{
    int row = list_click(app);

    if (row == -1)
        return;
    app->edit_song = app->libscene->view[row];
    scene_open(app, EDIT);
}

void libscene_event(app_t *app, sfEvent *event)
{
    if (event->type == sfEvtMouseWheelScrolled)
        list_scroll(app, event->mouseWheelScroll.delta);
    if (event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseRight)
        right_click(app);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    if (bar_seek(app) == 0)
        click(app);
    app->wg->mode = WG_CLICK;
    shortcuts(app);
    library_tabs(app);
    app->wg->mode = WG_DRAW;
}

void libscene_update(app_t *app)
{
    int playing = app->player->music
        && sfMusic_getStatus(app->player->music) == sfPlaying;

    ui_show(app->libscene->ui, LIB_PLAY, !playing);
    ui_show(app->libscene->ui, LIB_PAUSE, playing);
    list_update(app);
    bar_update(app);
}

void libscene_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->libscene->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->libscene->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    list_draw(app);
    bar_draw(app);
    ui_draw(app, app->libscene->ui);
    shortcuts(app);
    library_tabs(app);
}
