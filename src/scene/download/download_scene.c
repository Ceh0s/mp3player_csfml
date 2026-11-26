/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** download screen: the addresses of videos or sounds are pasted
** (separated by ;), their audio is saved as mp3 in music/ then added to
** the library and to the playlists chosen. The download goes on when
** leaving the screen (it belongs to app).
*/

#include "mp3player.h"

int dlscene_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};

    app->dlscene = calloc(1, sizeof(dlscene_t));
    if (!app->dlscene)
        return 84;
    app->dlscene->bg = background_create(BG_BLUR_PATH,
        &app->dlscene->bg_texture);
    app->dlscene->panel = rect_create(panel, &dark);
    if (!app->dlscene->bg || !app->dlscene->panel)
        return 84;
    dlscene_preselect(app);
    dlscene_range_load(app);
    if (app->sync_playlist)
        dlscene_sync(app);
    return 0;
}

/*
** escape closes the list shown over the screen, else leaves the screen
*/
static void key_pressed(app_t *app, sfKeyCode key)
{
    if (key != sfKeyEscape)
        return;
    if (app->dlscene->popup != DL_POPUP_NONE)
        app->dlscene->popup = DL_POPUP_NONE;
    else
        scene_back(app);
}

static void wheel(app_t *app, float delta)
{
    scroll_by(&app->dlscene->list, -delta * 2 * DLPOPUP_STEP);
}

/*
** 13 = enter: starts the download of the typed addresses
*/
void dlscene_event(app_t *app, sfEvent *event)
{
    if (event->type == sfEvtKeyPressed)
        key_pressed(app, event->key.code);
    if (event->type == sfEvtMouseWheelScrolled)
        wheel(app, event->mouseWheelScroll.delta);
    if (event->type == sfEvtTextEntered && event->text.unicode == 13)
        dlscene_start(app);
    else if (event->type == sfEvtTextEntered)
        dlscene_text(app, event->text.unicode);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    dlscene_view(app);
    app->wg->mode = WG_DRAW;
}

/*
** the list over the screen scrolls; nothing to scroll without it
*/
void dlscene_update(app_t *app)
{
    if (app->dlscene->popup == DL_POPUP_NONE)
        return;
    scroll_grab(app, &app->dlscene->list);
    scroll_update(app, &app->dlscene->list);
}

void dlscene_draw(app_t *app)
{
    background_draw(app, app->dlscene->bg);
    sfRenderWindow_drawRectangleShape(app->window, app->dlscene->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    dlscene_view(app);
    widget_tip_draw(app);
}

void dlscene_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->dlscene->bg);
    sfRectangleShape_setSize(app->dlscene->panel, size);
}

void dlscene_destroy(app_t *app)
{
    if (!app->dlscene)
        return;
    dlscene_range_save(app);
    if (app->dlscene->bg)
        sfSprite_destroy(app->dlscene->bg);
    if (app->dlscene->bg_texture)
        sfTexture_destroy(app->dlscene->bg_texture);
    if (app->dlscene->panel)
        sfRectangleShape_destroy(app->dlscene->panel);
    free(app->dlscene);
    app->dlscene = NULL;
}
