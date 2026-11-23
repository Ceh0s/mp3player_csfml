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
    app->libscene->drag_row = -1;
    app->libscene->sel.last = -1;
    app->libscene->bg = background_create(BG_BLUR_PATH,
        &app->libscene->bg_texture);
    app->libscene->ui = ui_create(app->atlas, lib_ui, 6);
    app->libscene->panel = rect_create(panel, &white);
    if (!app->libscene->bg || !app->libscene->ui || !app->libscene->panel
        || list_create(app) == 84 || bar_create(app) == 84)
        return 84;
    return 0;
}

/*
** in the selection mode, a click on a song checks it instead of
** playing it
*/
static void click(app_t *app)
{
    int id = ui_hit(app, app->libscene->ui);
    int row = app->libscene->selecting ? select_row(app) : list_click(app);

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
    if (app->libscene->selecting)
        select_click(app, row);
    else
        list_play(app, row);
}

/*
** shortcuts to the download, battle, rankings and statistics screens;
** a playlist linked to an address can be downloaded again
*/
static void shortcuts(app_t *app)
{
    float sync[4] = {1000, 20, 220, 56};
    source_t *src = source_get(app->lib, app->libscene->playlist_id);

    if (src && widget_button(app, sync, "Synchroniser")) {
        app->sync_playlist = src->playlist_id;
        scene_open(app, DOWNLOAD);
    }
    if (src)
        widget_tip(app, sync, src->url);
    if (widget_button(app, (float [4]){1235, 20, 220, 56}, "Télécharger"))
        scene_open(app, DOWNLOAD);
    if (widget_button(app, (float [4]){1470, 20, 150, 56}, "Battle"))
        scene_open(app, BATTLE);
    if (widget_button(app, (float [4]){1635, 20, 230, 56}, "Classements"))
        scene_open(app, RANKING);
    if (widget_button(app, (float [4]){765, 20, 220, 56}, "Statistiques"))
        scene_open(app, STATS);
}

/*
** the buttons around the list (drawn, or tested for a click); the
** search filters the list in both modes
*/
static void widgets(app_t *app)
{
    float search[4] = {1350, SORT_Y, 480, 52};

    library_likes(app);
    shortcuts(app);
    library_play_modes(app);
    library_select_toggle(app);
    library_tabs(app);
    if (app->libscene->selecting) {
        search[0] = 1000;
        search[2] = 560;
        library_select_bar(app);
    } else
        library_sort_bar(app);
    if (!app->libscene->sel.open)
        search_field(app, search);
}

static void right_click(app_t *app)
{
    int row = list_click(app);

    if (row == -1 || app->libscene->sel.open)
        return;
    app->edit_song = app->libscene->view[row];
    scene_open(app, EDIT);
}

/*
** the release of a dragged song is not a click; the actions window
** takes the clicks while it is open
*/
static void left_release(app_t *app)
{
    if (app->libscene->sel.open) {
        app->wg->mode = WG_CLICK;
        library_actions(app);
        app->wg->mode = WG_DRAW;
        return;
    }
    if (drag_release(app))
        return;
    if (bar_seek(app) == 0)
        click(app);
    app->wg->mode = WG_CLICK;
    widgets(app);
    app->wg->mode = WG_DRAW;
}

/*
** escape closes the actions window, then leaves the selection mode
*/
static void key(app_t *app, sfEvent const *event)
{
    libscene_t *scene = app->libscene;

    if (event->type == sfEvtTextEntered && scene->sel.open)
        library_actions_key(app, event->text.unicode);
    if (event->type != sfEvtKeyPressed || event->key.code != sfKeyEscape)
        return;
    if (scene->sel.open)
        scene->sel.open = 0;
    else if (scene->selecting) {
        scene->selecting = 0;
        select_all(app, 0);
    } else
        scene_back(app);
}

void libscene_event(app_t *app, sfEvent *event)
{
    key(app, event);
    if (event->type == sfEvtMouseWheelScrolled)
        list_scroll(app, event->mouseWheelScroll.delta);
    if (event->type == sfEvtMouseButtonPressed
        && event->mouseButton.button == sfMouseLeft
        && !app->libscene->selecting)
        drag_press(app);
    if (event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseRight)
        right_click(app);
    if (event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseLeft)
        left_release(app);
}

void libscene_update(app_t *app)
{
    int playing = app->player->music
        && sfMusic_getStatus(app->player->music) == sfPlaying;

    ui_show(app->libscene->ui, LIB_PLAY, !playing);
    ui_show(app->libscene->ui, LIB_PAUSE, playing);
    if (app->search.changed) {
        app->search.changed = 0;
        list_create(app);
    }
    list_update(app);
    drag_update(app);
    bar_update(app);
}

void libscene_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->libscene->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->libscene->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    list_draw(app);
    library_select_boxes(app);
    list_values(app);
    drag_draw(app);
    bar_draw(app);
    ui_draw(app, app->libscene->ui);
    widgets(app);
    if (app->libscene->sel.open)
        library_actions(app);
    widget_tip_draw(app);
}
