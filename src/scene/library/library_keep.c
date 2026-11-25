/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** the library is shown again as it was left (place in the list and
** search) when coming back from another screen, on the same tab; kept
** until the app closes (app->library_keep)
*/

#include "mp3player.h"

/*
** called when the screen is left: the place aimed at is kept, even if
** the list is still sliding to it
*/
void library_keep(app_t *app)
{
    library_keep_t *keep = &app->library_keep;

    keep->playlist_id = app->libscene->playlist_id;
    keep->scroll = app->libscene->scroll_to;
    snprintf(keep->search, NAME_LEN, "%s", app->search.text);
}

static int same_tab(app_t *app)
{
    return app->library_keep.playlist_id == app->libscene->playlist_id;
}

/*
** before the list is built, so that it is filtered
*/
void library_restore_search(app_t *app)
{
    if (same_tab(app))
        snprintf(app->search.text, NAME_LEN, "%s", app->library_keep.search);
}

/*
** once the list is built; libscene_layout brings the place back to the
** end of the list if it is shorter than before
*/
void library_restore_scroll(app_t *app)
{
    if (!same_tab(app))
        return;
    app->libscene->scroll = app->library_keep.scroll;
    app->libscene->scroll_to = app->library_keep.scroll;
}
