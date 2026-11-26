/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the playlist screen: name, search, list of the songs to
** check or uncheck, import of the dates, removal (called to draw it and to handle a click)
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
    scroll_jump(&pl->list, 0);
    for (int i = 0; i < app->lib->nb; i++) {
        song = app->lib->present[i];
        if (!str_contains(song_title(song), pl->search) && (!song->artists
                || !str_contains(song->artists, pl->search)))
            continue;
        pl->found[pl->nb_found] = song;
        pl->nb_found++;
    }
}

/*
** the wheel moves two rows
*/
void pledit_scroll(app_t *app, float delta)
{
    scroll_by(&app->pledit->list, -delta * 2 * PL_ROW_STEP);
}

static void header(app_t *app, pledit_t *pl)
{
    char count[128];

    widget_text(app, "Playlist", (float [3]){130, 115, 40}, &sfWhite);
    widget_text(app, "Nom", (float [3]){130, 172, 24}, &sfWhite);
    if (widget_field(app, (float [4]){130, 205, 900, 56}, pl->name,
            pl->focus == 0))
        pl->focus = 0;
    widget_text(app, "Rechercher", (float [3]){1080, 172, 24},
        &sfWhite);
    if (widget_field(app, (float [4]){1080, 205, 700, 56}, pl->search,
            pl->focus == 1))
        pl->focus = 1;
    snprintf(count, sizeof(count), "%d titre%s",
        pl->playlist->nb, pl->playlist->nb > 1 ? "s" : "");
    widget_text(app, count, (float [3]){130, 278, 22}, &gray);
}

/*
** the songs found, checked if in the playlist; they scroll
*/
static void song_rows(app_t *app, pledit_t *pl)
{
    song_t *song = NULL;
    float y = 0;
    float text[4] = {210, 0, 1550, PL_ROW_STEP - 12};
    float area[4] = {PANEL_X, PL_ROWS_Y, PANEL_W,
        PL_ROWS_BOTTOM + app->extra_h - PL_ROWS_Y};

    scroll_frame(&pl->list, area, pl->nb_found * PL_ROW_STEP);
    scroll_bar_draw(app, &pl->list);
    if (!scroll_rows_begin(app, &pl->list))
        return;
    for (int i = 0; i < pl->nb_found; i++) {
        song = pl->found[i];
        y = scroll_y(&pl->list, i * PL_ROW_STEP);
        if (!scroll_seen(&pl->list, y, PL_ROW_STEP))
            continue;
        if (widget_button(app, (float [4]){130, y, 1650, PL_ROW_STEP - 6},
                playlist_has_song(app->lib, pl->playlist, song) ? "[x]"
                : "[  ]"))
            playlist_toggle(app->lib, pl->playlist, song);
        text[1] = y + 4;
        widget_song(app, song, text, &sfWhite);
    }
    scroll_rows_end(app);
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
    widget_text(app, "Ses dates d'ajout et les règles de classement qui "
        "l'utilisent seront retirées.",
        (float [3]){130, 190, 24}, &gray);
    if (widget_button(app, (float [4]){130, y, 260, 56}, "Supprimer")) {
        rules_forget(app->lib, RULE_PLAYLIST, pl->playlist->id);
        playlist_remove(app->lib, pl->playlist);
        boards_sync_playlists(app->lib);
        pl->playlist = NULL;
        app->library_tab = 0;
        scene_back(app);
        return;
    }
    if (widget_button(app, (float [4]){410, y, 260, 56}, "Annuler"))
        pl->confirm = 0;
}

/*
** address of a site linked to the playlist ("Synchroniser" in the
** library), set in the download screen
*/
static void linked(app_t *app, pledit_t *pl, float y)
{
    source_t *src = source_get(app->lib, pl->playlist->id);
    char line[NAME_LEN + 16];
    char cut[64 + 4];

    if (!src)
        return;
    utf8_cut(cut, src->url, 64);
    snprintf(line, sizeof(line), "Liée à : %s", cut);
    widget_text(app, line, (float [3]){420, y + 14, 22}, &gray);
    if (!widget_button(app, (float [4]){1240, y, 170, 56}, "Délier"))
        return;
    source_remove(app->lib, pl->playlist->id);
    db_save_sources(app->lib);
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
    pledit_takeout(app);
    if (widget_back(app))
        scene_back(app);
    linked(app, pl, y);
    if (widget_icon_button(app, (float [4]){1430, y, 350, 56}, ICON_TRASH,
            "Supprimer la playlist"))
        pl->confirm = 1;
}
