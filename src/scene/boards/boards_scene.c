/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** management of the rankings: list of the rankings, edition of the
** rules of one of them, confirmation before a removal
*/

#include "mp3player.h"

int boards_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};

    app->boards = calloc(1, sizeof(boards_t));
    if (!app->boards)
        return 84;
    app->boards->bg = background_create(BG_BLUR_PATH,
        &app->boards->bg_texture);
    app->boards->panel = rect_create(panel, &dark);
    if (!app->boards->bg || !app->boards->panel)
        return 84;
    return 0;
}

static void build(app_t *app)
{
    void (*const screens[3])(app_t *) = {&boards_list, &boards_edit,
        &boards_confirm};

    screens[app->boards->mode](app);
}

/*
** escape goes back to the list from the edition of a ranking
*/
void boards_event(app_t *app, sfEvent *event)
{
    boards_t *bd = app->boards;

    if (event->type == sfEvtTextEntered)
        boards_text(app, event->text.unicode);
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyEscape) {
        if (bd->mode == BOARDS_LIST)
            scene_back(app);
        boards_leave(app);
    }
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    build(app);
    app->wg->mode = WG_DRAW;
}

void boards_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->boards->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->boards->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    build(app);
    widget_tip_draw(app);
}

void boards_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->boards->bg);
    sfRectangleShape_setSize(app->boards->panel, size);
}

void boards_destroy(app_t *app)
{
    if (!app->boards)
        return;
    boards_leave(app);
    if (app->boards->bg)
        sfSprite_destroy(app->boards->bg);
    if (app->boards->bg_texture)
        sfTexture_destroy(app->boards->bg_texture);
    if (app->boards->panel)
        sfRectangleShape_destroy(app->boards->panel);
    free(app->boards);
    app->boards = NULL;
    db_save_all(app->lib);
}
