/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** files of the duplicates screen whose sound is exactly the one of a
** song (same fingerprint, see same_files.c): the song keeps one of
** them, the other can be removed from music/ (asked twice)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

int dup_same_count(app_t *app)
{
    int nb = 0;

    for (same_file_t *same = app->lib->same_files; same; same = same->next)
        nb += search_text(app, same->file) || search_song(app,
            song_resolve(app->lib, song_by_id(app->lib, same->song_id)));
    return nb;
}

/*
** the n-th file shown (the search leaves some out), NULL if none
*/
same_file_t *dup_same_get(app_t *app, int n)
{
    song_t *song = NULL;

    for (same_file_t *same = app->lib->same_files; same; same = same->next) {
        song = song_resolve(app->lib, song_by_id(app->lib, same->song_id));
        if (!search_text(app, same->file) && !search_song(app, song))
            continue;
        if (n == 0)
            return same;
        n--;
    }
    return NULL;
}

static void texts(app_t *app, same_file_t const *same, float y)
{
    song_t *song = song_by_id(app->lib, same->song_id);
    char line[NAME_LEN + 64];
    char cut[64 + 4];
    char const *name = NULL;

    song = song_resolve(app->lib, song);
    utf8_cut(cut, song ? song_title(song) : "?", 60);
    snprintf(line, sizeof(line), "Son identique à « %s »", cut);
    widget_text(app, line, (float [3]){130, y + 2, 22}, &sfWhite);
    name = strrchr(same->path, '/');
    utf8_cut(cut, name ? name + 1 : same->path, 64);
    widget_text(app, cut, (float [3]){150, y + 30, 18}, &gray);
}

/*
** returns 1 if the file was removed (the list is rebuilt)
*/
int dup_same_row(app_t *app, int n, float y)
{
    dupscene_t *scene = app->dupscene;
    same_file_t *same = dup_same_get(app, n);
    float rect[4] = {1380, y + 2, 400, 44};
    int asked = scene->confirm == n + 1;

    if (!same)
        return 0;
    texts(app, same, y);
    widget_help(app, rect, "Efface ce fichier de music/ : la musique garde "
        "l'autre, au son identique");
    if (!widget_button(app, rect, asked ? "Confirmer l'effacement"
            : "Effacer ce fichier"))
        return 0;
    scene->confirm = asked ? 0 : n + 1;
    if (!asked)
        return 0;
    same_file_delete(same);
    library_drop_missing(app);
    dupscene_clean(app);
    return 1;
}
