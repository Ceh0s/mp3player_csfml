/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page "À vérifier" of the list over the download screen: artist names
** written as a list that may be one artist ("Tyler, The Creator"): the
** proposed split, a song that has it, and "Séparer" / "Garder"
** (artist_split.c; the other names are split on their own)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

int dlscene_split_count(app_t *app)
{
    int nb = 0;

    for (artist_t *a = app->lib->artists; a; a = a->next)
        nb += artist_split_state(app->lib, a) == 2;
    return nb;
}

/*
** "A / B / C" and the title of a song of the artist
*/
static void describe(app_t *app, artist_t const *artist, float y)
{
    char parts[SPLIT_MAX][NAME_LEN];
    int nb = artist_split_parts(artist->name, parts);
    char line[NAME_LEN * 2];
    song_t *song = NULL;

    line[0] = '\0';
    for (int i = 0; i < nb; i++) {
        strncat(line, i > 0 ? "  /  " : "en :  ", sizeof(line) - strlen(line) - 1);
        strncat(line, parts[i], sizeof(line) - strlen(line) - 1);
    }
    widget_fit(app, artist->name, (float [4]){300, y - 2, 22, 380}, &sfWhite);
    widget_fit(app, line, (float [4]){700, y - 2, 22, 440}, &sfWhite);
    for (link_t *link = app->lib->links; link && !song; link = link->next)
        if (link->artist_id == artist->id)
            song = song_by_id(app->lib, link->song_id);
    if (song)
        widget_fit(app, song_title(song), (float [4]){300, y + 24, 16, 840},
            &gray);
}

/*
** returns 1 once a choice is made (the list changes)
*/
static int choose(app_t *app, artist_t *artist, float y)
{
    if (widget_button(app, (float [4]){1180, y, 220, 44}, "Séparer")) {
        split_choice_set(app->lib, artist->name, 0);
        library_changed(app);
        return 1;
    }
    if (widget_button(app, (float [4]){1420, y, 235, 44}, "Garder entier")) {
        split_choice_set(app->lib, artist->name, 1);
        db_save_splits(app->lib);
        return 1;
    }
    return 0;
}

void dlscene_splits(app_t *app)
{
    int i = -1;
    float y = 0;

    for (artist_t *a = app->lib->artists; a; a = a->next) {
        if (artist_split_state(app->lib, a) != 2
            || !search_text(app, a->name) || !dlpopup_place(app, &i, &y))
            continue;
        describe(app, a, y);
        if (choose(app, a, y))
            return;
    }
}
