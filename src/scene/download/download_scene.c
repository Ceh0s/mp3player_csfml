/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** download screen: the address of a video or a sound is pasted, its
** audio is saved as an mp3 in music/ then added to the library. The
** download goes on when leaving the screen (it belongs to app).
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
    return 0;
}

/*
** 13 = enter: starts the download of the typed address
*/
void dlscene_event(app_t *app, sfEvent *event)
{
    if (event->type == sfEvtTextEntered && event->text.unicode == 13)
        dlscene_start(app);
    else if (event->type == sfEvtTextEntered)
        text_input(app->dlscene->url, event->text.unicode);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    dlscene_view(app);
    app->wg->mode = WG_DRAW;
}

void dlscene_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->dlscene->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->dlscene->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    dlscene_view(app);
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
    if (app->dlscene->bg)
        sfSprite_destroy(app->dlscene->bg);
    if (app->dlscene->bg_texture)
        sfTexture_destroy(app->dlscene->bg_texture);
    if (app->dlscene->panel)
        sfRectangleShape_destroy(app->dlscene->panel);
    free(app->dlscene);
    app->dlscene = NULL;
}
