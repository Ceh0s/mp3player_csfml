/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** edit screen of a song (title, artists, merge), opened with a right
** click in the library
*/

#include "mp3player.h"

static void load_role(app_t *app, edit_t *edit, role_t role)
{
    artist_t *artist = NULL;
    artist_role_t *slot = NULL;

    for (link_t *link = app->lib->links; link; link = link->next) {
        if (link->song_id != edit->song->id || link->role != role
            || edit->nb_artists >= EDIT_ARTISTS)
            continue;
        artist = artist_by_id(app->lib, link->artist_id);
        if (!artist)
            continue;
        slot = &edit->artists[edit->nb_artists];
        snprintf(slot->name, NAME_LEN, "%s", artist->name);
        slot->role = role;
        edit->nb_artists++;
    }
}

int edit_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};

    app->edit = calloc(1, sizeof(edit_t));
    if (!app->edit || !app->edit_song)
        return 84;
    app->edit->song = app->edit_song;
    app->edit->bg = background_create(BG_BLUR_PATH, &app->edit->bg_texture);
    app->edit->panel = rect_create(panel, &dark);
    if (!app->edit->bg || !app->edit->panel)
        return 84;
    snprintf(app->edit->title, NAME_LEN, "%s", song_title(app->edit->song));
    for (int role = 0; role < NB_ROLES; role++)
        load_role(app, app->edit, role);
    return 0;
}

static void build(app_t *app)
{
    void (*const screens[5])(app_t *) = {&edit_main, &edit_search,
        &edit_confirm, &edit_artist, &edit_titles};

    screens[app->edit->mode](app);
}

void edit_event(app_t *app, sfEvent *event)
{
    edit_t *edit = app->edit;

    if (event->type == sfEvtTextEntered)
        edit_text_entered(app, event->text.unicode);
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyEscape) {
        if (edit->mode == EDIT_MAIN)
            scene_back(app);
        edit->mode = edit->mode == EDIT_CONFIRM ? EDIT_SEARCH : EDIT_MAIN;
    }
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    build(app);
    app->wg->mode = WG_DRAW;
}

void edit_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->edit->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->edit->panel, NULL);
    app->wg->mode = WG_DRAW;
    build(app);
    widget_tip_draw(app);
}

void edit_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->edit->bg);
    sfRectangleShape_setSize(app->edit->panel, size);
}

void edit_destroy(app_t *app)
{
    if (!app->edit)
        return;
    if (app->edit->bg)
        sfSprite_destroy(app->edit->bg);
    if (app->edit->bg_texture)
        sfTexture_destroy(app->edit->bg_texture);
    if (app->edit->panel)
        sfRectangleShape_destroy(app->edit->panel);
    free(app->edit);
    app->edit = NULL;
}
