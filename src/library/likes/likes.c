/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** likes and dislikes of the songs and artists (likes.tsv): raw ids,
** resolved when read (a like given to a song merged since then counts
** for the song it was merged into)
*/

#include "mp3player.h"

/*
** returns 1 (like), -1 (dislike) or 0
*/
int like_get(library_t *lib, elo_kind_t kind, int id)
{
    for (like_t *like = lib->likes; like; like = like->next)
        if (like->kind == (int)kind
            && subject_resolve(lib, kind, like->id) == id)
            return like->value;
    return 0;
}

/*
** forgets every like that counts for id (an alias may have one)
*/
static void like_drop(library_t *lib, elo_kind_t kind, int id)
{
    like_t **link = &lib->likes;
    like_t *like = NULL;

    while (*link) {
        like = *link;
        if (like->kind == (int)kind
            && subject_resolve(lib, kind, like->id) == id) {
            *link = like->next;
            free(like);
        } else
            link = &like->next;
    }
}

/*
** appended at the end: the automatic playlists follow the order in which
** the songs were liked; returns 84 on error
*/
int like_add(library_t *lib, elo_kind_t kind, int id, int value)
{
    like_t **last = &lib->likes;
    like_t *like = calloc(1, sizeof(like_t));

    if (!like)
        return 84;
    like->kind = kind;
    like->id = id;
    like->value = value;
    while (*last)
        last = &(*last)->next;
    *last = like;
    return 0;
}

/*
** value : 1 like, -1 dislike, 0 none; the same value again removes it
** (the button works as a toggle); the automatic playlists follow
*/
/*
** value is set as it is (0 = no opinion), nothing is saved: used for
** several songs at once
*/
void like_put(library_t *lib, elo_kind_t kind, int id, int value)
{
    like_drop(lib, kind, id);
    if (value != 0)
        like_add(lib, kind, id, value);
}

/*
** giving the same value again removes it; saved at once
*/
void like_set(library_t *lib, elo_kind_t kind, int id, int value)
{
    int old = like_get(lib, kind, id);

    like_put(lib, kind, id, value == old ? 0 : value);
    likes_sync(lib);
    db_save_likes(lib);
    db_save_playlists(lib);
}

void likes_free(library_t *lib)
{
    like_t *next = NULL;

    while (lib->likes) {
        next = lib->likes->next;
        free(lib->likes);
        lib->likes = next;
    }
}
