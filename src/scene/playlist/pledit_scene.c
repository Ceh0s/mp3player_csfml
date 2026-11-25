/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** edition of a playlist: its name and its songs (checked in the list
** of the present songs), opened from the tabs of the library
*/

#include "mp3player.h"

/*
** app->edit_playlist = NULL creates a new playlist
*/
int pledit_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};
    pledit_t *pl = calloc(1, sizeof(pledit_t));

    app->pledit = pl;
    if (!pl)
        return 84;
    pl->bg = background_create(BG_BLUR_PATH, &pl->bg_texture);
    pl->panel = rect_create(panel, &dark);
    pl->playlist = app->edit_playlist ? app->edit_playlist
        : playlist_add(app->lib, app->lib->next_playlist,
        "Nouvelle playlist");
    pl->found = malloc(sizeof(song_t *) * (app->lib->nb + 1));
    if (!pl->bg || !pl->panel || !pl->playlist || !pl->found)
        return 84;
    snprintf(pl->name, NAME_LEN, "%s", pl->playlist->name);
    app->library_tab = pl->playlist->id;
    pledit_search(app);
    return 0;
}

/*
** another path has to be confirmed again (pledit_takeout.c)
*/
static void typed(app_t *app, pledit_t *pl)
{
    if (pl->focus == 1)
        pledit_search(app);
    if (pl->focus == 2) {
        pl->takeout_ask = 0;
        pl->takeout_msg[0] = '\0';
    }
}

/*
** tab switches between the fields (name, search, takeout)
*/
void pledit_event(app_t *app, sfEvent *event)
{
    pledit_t *pl = app->pledit;
    sfUint32 code = event->text.unicode;
    char *fields[3] = {pl->name, pl->search, pl->takeout};
    char *field = fields[pl->focus];

    if (event->type == sfEvtTextEntered && code == 9)
        pl->focus = (pl->focus + 1) % 3;
    else if (event->type == sfEvtTextEntered
        && text_input(app, field, code))
        typed(app, pl);
    if (event->type == sfEvtMouseWheelScrolled)
        pledit_scroll(app, event->mouseWheelScroll.delta);
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    pledit_view(app);
    app->wg->mode = WG_DRAW;
}

void pledit_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->pledit->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->pledit->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    pledit_view(app);
}

void pledit_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->pledit->bg);
    sfRectangleShape_setSize(app->pledit->panel, size);
    pledit_scroll(app, 0);
}

/*
** the typed name is applied if not empty
*/
static void apply_name(pledit_t *pl)
{
    char *name = NULL;

    if (pl->playlist && pl->name[0] != '\0')
        name = strdup(pl->name);
    if (!name)
        return;
    free(pl->playlist->name);
    pl->playlist->name = name;
}

/*
** everything is saved and the player follows the new content of its
** playlist
*/
void pledit_destroy(app_t *app)
{
    pledit_t *pl = app->pledit;

    if (!pl)
        return;
    apply_name(pl);
    if (pl->bg)
        sfSprite_destroy(pl->bg);
    if (pl->bg_texture)
        sfTexture_destroy(pl->bg_texture);
    if (pl->panel)
        sfRectangleShape_destroy(pl->panel);
    free(pl->found);
    free(pl);
    app->pledit = NULL;
    app->edit_playlist = NULL;
    boards_sync_playlists(app->lib);
    player_resync(app);
    db_save_all(app->lib);
}
