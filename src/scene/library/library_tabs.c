/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** tabs of the library: "Tous les titres", one per playlist and
** "+ Playlist" to create one; "Modifier la playlist" opens the shown one
** (drawn with the widgets)
*/

#include "mp3player.h"

/*
** a tab is as wide as its name; the tabs after TABS_RIGHT are not shown
*/
static int tab(app_t *app, float *x, char const *name, int selected)
{
    char label[NAME_LEN + 8];
    float width = 0;
    int clicked = 0;

    snprintf(label, sizeof(label), selected ? "> %s <" : "%s", name);
    width = widget_text_width(app, label, 26) + 32;
    if (*x + width > TABS_RIGHT)
        return 0;
    clicked = widget_button(app, (float [4]){*x, TABS_Y, width, 52}, label);
    *x += width + 12;
    return clicked;
}

static void edit_playlist(app_t *app, int id)
{
    app->edit_playlist = playlist_by_id(app->lib, id);
    scene_open(app, PLAYLIST);
}

void library_tabs(app_t *app)
{
    libscene_t *scene = app->libscene;
    float x = LIST_X;
    int id = scene->playlist_id;

    if (tab(app, &x, "Tous les titres", id == 0))
        id = 0;
    for (playlist_t *pl = app->lib->playlists; pl; pl = pl->next)
        if (tab(app, &x, pl->name, pl->id == scene->playlist_id))
            id = pl->id;
    if (tab(app, &x, "+ Playlist", 0))
        edit_playlist(app, 0);
    if (scene->playlist_id != 0 && widget_button(app,
            (float [4]){1560, TABS_Y, 250, 52}, "Modifier la playlist"))
        edit_playlist(app, scene->playlist_id);
    if (id == scene->playlist_id)
        return;
    scene->playlist_id = id;
    app->library_tab = id;
    list_create(app);
}
