/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the playlist screen: name, search, list of the songs to
** check or uncheck, removal (called to draw it and to handle a click)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

/*
** the present songs whose title or artists contain the search
*/
void pledit_search(app_t *app)
{
    pledit_t *pl = app->pledit;
    song_t *song = NULL;

    pl->nb_found = 0;
    pl->scroll = 0;
    for (int i = 0; i < app->lib->nb; i++) {
        song = app->lib->present[i];
        if (!str_contains(song_title(song), pl->search) && (!song->artists
                || !str_contains(song->artists, pl->search)))
            continue;
        pl->found[pl->nb_found] = song;
        pl->nb_found++;
    }
}

static int visible_rows(app_t *app)
{
    return (CONTENT_BOTTOM + app->extra_h - PL_ROWS_Y) / PL_ROW_STEP;
}

/*
** delta = 0 only keeps the scroll valid (after a resize)
*/
void pledit_scroll(app_t *app, float delta)
{
    pledit_t *pl = app->pledit;
    int max = pl->nb_found - visible_rows(app);

    pl->scroll -= (int)delta;
    if (pl->scroll > max)
        pl->scroll = max;
    if (pl->scroll < 0)
        pl->scroll = 0;
}

static void header(app_t *app, pledit_t *pl)
{
    char count[128];

    widget_text(app, "Playlist", (float [3]){130, 115, 40}, &sfWhite);
    widget_text(app, "Nom", (float [3]){130, 172, 24}, &sfWhite);
    if (widget_field(app, (float [4]){130, 205, 900, 56}, pl->name,
            pl->focus == 0))
        pl->focus = 0;
    widget_text(app, "Rechercher un titre ou un artiste",
        (float [3]){1080, 172, 24}, &sfWhite);
    if (widget_field(app, (float [4]){1080, 205, 700, 56}, pl->search,
            pl->focus == 1))
        pl->focus = 1;
    snprintf(count, sizeof(count), "%d titre%s dans la playlist   -   "
        "clique sur un titre pour l'ajouter ou le retirer",
        pl->playlist->nb, pl->playlist->nb > 1 ? "s" : "");
    widget_text(app, count, (float [3]){130, 278, 22}, &gray);
}

static void song_rows(app_t *app, pledit_t *pl)
{
    char label[NAME_LEN * 2 + 16];
    song_t *song = NULL;
    int rows = visible_rows(app);
    float y = 0;

    for (int i = pl->scroll; i < pl->nb_found && i < pl->scroll + rows;
        i++) {
        song = pl->found[i];
        y = PL_ROWS_Y + (i - pl->scroll) * PL_ROW_STEP;
        snprintf(label, sizeof(label), "[%s]   %s%s%s",
            playlist_has_song(app->lib, pl->playlist, song) ? "x" : "  ",
            song_title(song), song->artists ? " - " : "",
            song->artists ? song->artists : "");
        if (widget_button(app, (float [4]){130, y, 1650, 46}, label))
            playlist_toggle(app->lib, pl->playlist, song);
    }
}

/*
** the rules of the rankings using the playlist are removed with it
*/
static void confirm(app_t *app, pledit_t *pl)
{
    char line[NAME_LEN + 64];
    float y = 840 + app->extra_h;

    snprintf(line, sizeof(line), "Supprimer la playlist « %s » ?",
        pl->playlist->name);
    widget_text(app, line, (float [3]){130, 115, 40}, &sfWhite);
    widget_text(app, "Les musiques et leurs statistiques ne sont pas "
        "touchées. Les règles de classement qui l'utilisent sont retirées.",
        (float [3]){130, 190, 24}, &gray);
    if (widget_button(app, (float [4]){130, y, 260, 56}, "Supprimer")) {
        rules_forget(app->lib, RULE_PLAYLIST, pl->playlist->id);
        playlist_remove(app->lib, pl->playlist);
        pl->playlist = NULL;
        app->library_tab = 0;
        scene_back(app);
        return;
    }
    if (widget_button(app, (float [4]){410, y, 260, 56}, "Annuler"))
        pl->confirm = 0;
}

void pledit_view(app_t *app)
{
    pledit_t *pl = app->pledit;
    float y = 840 + app->extra_h;

    if (pl->confirm) {
        if (pl->playlist)
            confirm(app, pl);
        return;
    }
    header(app, pl);
    song_rows(app, pl);
    if (widget_button(app, (float [4]){130, y, 260, 56}, "Retour"))
        scene_back(app);
    if (widget_button(app, (float [4]){1430, y, 350, 56},
            "Supprimer la playlist"))
        pl->confirm = 1;
}
