/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a suggestion applied: its title, and the artist it adds. An artist
** taken from "X - Title" that the song did not have replaces the main
** artists (they were the uploader of the video); a featuring or a
** remixer is added
*/

#include "mp3player.h"
#include <strings.h>

static void insert_main(artist_role_t *roles, int *nb, char const *name)
{
    int kept = 0;

    for (int i = 0; i < *nb; i++)
        if (roles[i].role != ROLE_MAIN) {
            roles[kept] = roles[i];
            kept++;
        }
    if (kept >= EDIT_ARTISTS)
        kept = EDIT_ARTISTS - 1;
    memmove(&roles[1], &roles[0], sizeof(artist_role_t) * kept);
    snprintf(roles[0].name, NAME_LEN, "%s", name);
    roles[0].role = ROLE_MAIN;
    *nb = kept + 1;
}

/*
** roles : the artists of the song (EDIT_ARTISTS at most), nb of them;
** an artist already there takes the role of a featuring or a remixer
** ("Title (X Remix)" by X: X is the remixer, not the artist)
*/
void suggest_apply_roles(artist_role_t *roles, int *nb, suggest_t const *sug)
{
    if (sug->artist[0] == '\0')
        return;
    suggest_split_names(roles, nb, sug->artist);
    for (int i = 0; i < *nb; i++) {
        if (strcasecmp(roles[i].name, sug->artist) != 0)
            continue;
        if (sug->role != ROLE_MAIN)
            roles[i].role = sug->role;
        return;
    }
    if (sug->role == ROLE_MAIN) {
        insert_main(roles, nb, sug->artist);
        return;
    }
    if (*nb >= EDIT_ARTISTS)
        return;
    snprintf(roles[*nb].name, NAME_LEN, "%s", sug->artist);
    roles[*nb].role = sug->role;
    (*nb)++;
}

/*
** adds the artists of the song with this role after those in out
*/
static int add_role(library_t *lib, song_t const *song, int role,
    artist_role_t *out)
{
    artist_t *artist = NULL;
    int nb = 0;

    while (nb < EDIT_ARTISTS && out[nb].name[0] != '\0')
        nb++;
    for (link_t *link = lib->links; link; link = link->next) {
        artist = artist_by_id(lib, link->artist_id);
        if (link->song_id != song->id || (int)link->role != role
            || !artist || nb >= EDIT_ARTISTS)
            continue;
        snprintf(out[nb].name, NAME_LEN, "%s", artist->name);
        out[nb].role = role;
        nb++;
    }
    return nb;
}

/*
** the artists of the song (out holds EDIT_ARTISTS), main ones first;
** returns their number
*/
int song_roles(library_t *lib, song_t const *song, artist_role_t *out)
{
    int nb = 0;

    memset(out, 0, sizeof(artist_role_t) * EDIT_ARTISTS);
    for (int role = 0; role < NB_ROLES; role++)
        nb = add_role(lib, song, role, out);
    return nb;
}

/*
** index of the first suggestion neither hidden nor deleted, -1 if none
*/
int suggest_first(library_t *lib, song_t *song, char const *title,
    suggest_list_t *list)
{
    suggest_list(lib, song, title, list);
    for (int i = 0; i < list->nb; i++)
        if (suggest_state(lib, song->id, &list->items[i]) == SUG_VISIBLE)
            return i;
    return -1;
}

/*
** applies the visible suggestions one after the other (each one on the
** title left by the previous one); returns how many were applied
*/
int suggest_apply_song(library_t *lib, song_t *song)
{
    suggest_list_t list;
    artist_role_t roles[EDIT_ARTISTS];
    int nb = song_roles(lib, song, roles);
    char title[NAME_LEN];
    int applied = 0;
    int i = 0;

    snprintf(title, NAME_LEN, "%s", song_title(song));
    i = suggest_first(lib, song, title, &list);
    while (i >= 0 && applied < SUGGEST_MAX) {
        snprintf(title, NAME_LEN, "%s", list.items[i].title);
        suggest_apply_roles(roles, &nb, &list.items[i]);
        applied++;
        i = suggest_first(lib, song, title, &list);
    }
    if (applied == 0)
        return 0;
    song_set_title(song, title);
    song_set_artists(lib, song, roles, nb);
    return applied;
}
