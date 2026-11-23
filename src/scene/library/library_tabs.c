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
/*
** the tab shown has its number of present songs (found / all of them
** during a search)
*/
static void tab_label(app_t *app, char *label, char const *name,
    int selected)
{
    libscene_t *scene = app->libscene;

    snprintf(label, NAME_LEN + 32, "%s", name);
    if (selected && app->search.text[0] != '\0')
        snprintf(label, NAME_LEN + 32, "> %s (%d / %d) <", name,
            scene->nb_view, scene->nb_total);
    else if (selected)
        snprintf(label, NAME_LEN + 32, "> %s (%d) <", name, scene->nb_total);
}

static int tab(app_t *app, float *x, char const *name, int selected)
{
    char label[NAME_LEN + 32];
    float width = 0;
    int clicked = 0;

    tab_label(app, label, name, selected);
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

/*
** "Tous les titres" and the automatic playlists are not edited by hand
*/
static int editable(app_t *app, int id)
{
    playlist_t *pl = playlist_by_id(app->lib, id);

    return pl && pl->auto_like == 0;
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
    if (editable(app, scene->playlist_id) && widget_button(app,
            (float [4]){1560, TABS_Y, 250, 52}, "Modifier la playlist"))
        edit_playlist(app, scene->playlist_id);
    if (id == scene->playlist_id)
        return;
    scene->playlist_id = id;
    app->library_tab = id;
    list_create(app);
}
