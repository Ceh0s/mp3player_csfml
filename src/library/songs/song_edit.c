/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** edition of the title and artists of a song (the mp3 file is never
** modified), every change is written in data/edits.log
*/

#include "mp3player.h"

static void log_edit(song_t const *song, char const *field,
    char const *before, char const *after)
{
    char *line = malloc(64 + (before ? strlen(before) : 0)
        + (after ? strlen(after) : 0));

    if (!line)
        return;
    sprintf(line, "%d\t%s\t%s\t%s", song->id, field, before ? before : "",
        after ? after : "");
    log_line(EDITS_LOG, EDITS_COLUMNS, line);
    free(line);
}

void song_set_title(song_t *song, char const *title)
{
    char *old = song->title;

    if (old && strcmp(old, title) == 0)
        return;
    song->title = str_or_null(title);
    log_edit(song, "title", old, song->title);
    free(old);
}

void song_set_artists(library_t *lib, song_t *song,
    artist_role_t const *list, int nb)
{
    char *before = song->artists ? strdup(song->artists) : NULL;
    artist_t *artist = NULL;

    links_remove_song(lib, song->id);
    for (int i = nb - 1; i >= 0; i--) {
        artist = artist_get(lib, list[i].name);
        if (artist)
            link_add(lib, song->id, artist->id, list[i].role);
    }
    song_build_artists(lib, song);
    if ((before || song->artists) && (!before || !song->artists
            || strcmp(before, song->artists) != 0))
        log_edit(song, "artists", before, song->artists);
    free(before);
}

/*
** to call after any change of the songs: rebuilds the present songs,
** finds the current song again and saves everything
*/
void library_changed(app_t *app)
{
    library_refresh(app->lib);
    db_load_elo(app->lib);
    player_resync(app);
    db_save_all(app->lib);
}
