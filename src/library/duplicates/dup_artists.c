/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** names of the artists of a song, to compare two uploads: its artists,
** its tag (often the channel that uploaded the video) and the artist
** written in its title. The marks of an uploader are removed: "Madeon -
** Topic", "MadeonVEVO" and "Madeon Official" all give "madeon".
*/

#include "mp3player.h"

static char const *const uploader[] = {" topic", "vevo", " official",
    " officiel", NULL};

static void strip_uploader(char *name)
{
    size_t len = 0;
    size_t mark = 0;

    for (int i = 0; uploader[i]; i++) {
        len = strlen(name);
        mark = strlen(uploader[i]);
        if (len > mark && strcmp(name + len - mark, uploader[i]) == 0)
            name[len - mark] = '\0';
    }
    len = strlen(name);
    if (len > 0 && name[len - 1] == ' ')
        name[len - 1] = '\0';
}

/*
** adds "name|" to out (which ends with '|'), if not there yet
*/
static void add_name(char *out, char const *raw, int len)
{
    char name[NAME_LEN];
    char clean[NAME_LEN];
    char key[NAME_LEN + 2];

    snprintf(name, len + 1 < NAME_LEN ? len + 1 : NAME_LEN, "%s", raw);
    dup_clean(name, clean, NAME_LEN);
    strip_uploader(clean);
    snprintf(key, sizeof(key), "|%s|", clean);
    if (clean[0] == '\0' || strstr(out, key)
        || strlen(out) + strlen(clean) + 2 >= NAME_LEN * 2)
        return;
    strcat(out, clean);
    strcat(out, "|");
}

/*
** "A, B & C" or "A; B / C": each name is added
*/
static void add_names(char *out, char const *names)
{
    int len = 0;

    while (names && *names) {
        len = strcspn(names, ",;/&");
        add_name(out, names, len);
        names += len;
        names += *names != '\0';
    }
}

/*
** adds the names "A, B & C" to a list of names ("|a|b|" form, "|" when
** empty), used for a video of a listing (its channel)
*/
void dup_names(char *out, char const *names)
{
    add_names(out, names);
}

void dup_artists(library_t *lib, song_t const *song, char const *prefix,
    char *out)
{
    artist_t *artist = NULL;

    strcpy(out, "|");
    for (link_t *link = lib->links; link; link = link->next) {
        artist = link->song_id == song->id ?
            artist_by_id(lib, link->artist_id) : NULL;
        if (artist)
            add_names(out, artist->name);
    }
    add_names(out, song->src_artists);
    add_names(out, prefix);
}

/*
** 1 if a name is in both lists, or if one of them is empty (nothing
** known: the other measures decide)
*/
int dup_artists_meet(char const *a, char const *b)
{
    char key[NAME_LEN + 2];
    int len = 0;

    if (strcmp(a, "|") == 0 || strcmp(b, "|") == 0)
        return 1;
    for (a++; *a; a += len + 1) {
        len = strcspn(a, "|");
        if (len >= NAME_LEN)
            return 0;
        snprintf(key, len + 3, "|%s", a);
        if (strstr(b, key))
            return 1;
    }
    return 0;
}
