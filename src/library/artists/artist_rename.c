/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** renaming an artist (artist page): its new name is shown on all its
** songs. A name already taken by another artist merges them: the songs
** of the renamed one go to the other (with its like and image if the
** other has none), and the renamed one is removed. Its battles of
** artists stay on its old id (the Elo of artists are not merged yet).
** Its id is never given again (next_artist is kept in settings.tsv).
*/

#include "mp3player.h"

static void log_rename(artist_t const *artist, char const *name)
{
    char line[NAME_LEN * 2 + 64];

    snprintf(line, sizeof(line), "0\tartist %d\t%s\t%s", artist->id,
        artist->name, name);
    log_line(EDITS_LOG, EDITS_COLUMNS, line);
}

/*
** a song linked to both keeps one link, main if one of them was
*/
static void move_links(library_t *lib, int from, int to)
{
    link_t **cur = &lib->links;
    link_t *old = NULL;

    for (link_t *link = lib->links; link; link = link->next)
        if (link->artist_id == from)
            link_add(lib, link->song_id, to, link->role);
    for (link_t *link = lib->links; link; link = link->next) {
        if (link->artist_id != from || link->role != ROLE_MAIN)
            continue;
        for (link_t *other = lib->links; other; other = other->next)
            if (other->artist_id == to && other->song_id == link->song_id)
                other->role = ROLE_MAIN;
    }
    while (*cur) {
        if ((*cur)->artist_id != from) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old);
    }
}

/*
** the image of the removed artist is taken if the other one has none
*/
static void move_image(artist_t *from, artist_t *to)
{
    rank_t old = {NULL, from, 0, 0, 0};
    rank_t new = {NULL, to, 0, 0, 0};
    char src[PATH_LEN];
    char dst[PATH_LEN];

    for (int small = 0; small < 2; small++) {
        image_path(src, &old, small);
        image_path(dst, &new, small);
        if (access(dst, F_OK) != 0)
            rename(src, dst);
        else
            remove(src);
    }
}

static void remove_artist(library_t *lib, artist_t *artist)
{
    artist_t **cur = &lib->artists;

    while (*cur && *cur != artist)
        cur = &(*cur)->next;
    if (!*cur)
        return;
    *cur = artist->next;
    free(artist->name);
    free(artist);
}

static void merge(library_t *lib, artist_t *from, artist_t *to)
{
    int like = like_get(lib, KIND_ARTIST, from->id);

    move_links(lib, from->id, to->id);
    if (like != 0 && like_get(lib, KIND_ARTIST, to->id) == 0)
        like_put(lib, KIND_ARTIST, to->id, like);
    like_put(lib, KIND_ARTIST, from->id, 0);
    move_image(from, to);
    remove_artist(lib, from);
}

/*
** returns the artist now carrying the name (the other one after a
** merge), NULL if nothing changed
*/
artist_t *artist_rename(app_t *app, artist_t *artist, char const *name)
{
    char clean[NAME_LEN];
    artist_t *other = NULL;

    snprintf(clean, NAME_LEN, "%s", name);
    str_trim(clean);
    if (clean[0] == '\0' || strcmp(clean, artist->name) == 0)
        return NULL;
    log_rename(artist, clean);
    other = artist_find(app->lib, clean);
    if (other && other != artist) {
        merge(app->lib, artist, other);
        artist = other;
    } else {
        name = strdup(clean);
        if (!name)
            return NULL;
        free(artist->name);
        artist->name = (char *)name;
    }
    library_changed(app);
    db_save_settings(app->lib);
    return artist;
}
