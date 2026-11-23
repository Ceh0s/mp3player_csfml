/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** sort bar of the library, under the tabs: one button per criterion
** (clicking the chosen one again changes the direction), and the value
** of the criterion written at the right of each song
*/

#include "mp3player.h"

static const sfColor gray = {190, 190, 190, 255};

static int sort_button(app_t *app, float *x, int key,
    view_sort_t const *sort)
{
    char label[64];
    char const *name = sort_def(key)->label;
    float width = 0;
    int clicked = 0;

    if ((int)sort->key == key && key != SORT_CUSTOM)
        snprintf(label, sizeof(label), "> %s : %s <", name,
            sort->desc ? "décroissant" : "croissant");
    else
        snprintf(label, sizeof(label), (int)sort->key == key ? "> %s <"
            : "%s", name);
    width = widget_text_width(app, label, 26) + 32;
    clicked = widget_button(app, (float [4]){*x, SORT_Y, width, 52}, label);
    *x += width + 12;
    return clicked;
}

/*
** the list and, if it plays this playlist, the queue of the player take
** the new order (the scroll is kept after a song was moved)
*/
void library_reorder(app_t *app, int keep_scroll)
{
    libscene_t *scene = app->libscene;
    int scroll = scene->scroll;

    list_create(app);
    if (keep_scroll) {
        scene->scroll = scroll;
        list_scroll(app, 0);
    }
    if (app->player->queue && app->player->playlist_id == scene->playlist_id)
        player_resync(app);
}

/*
** "Ajout playlist" is the order of a playlist: "Tous les titres" has none
*/
void library_sort_bar(app_t *app)
{
    int id = app->libscene->playlist_id;
    view_sort_t *sort = sort_get(app->lib, id);
    float x = LIST_X;
    int chosen = -1;

    if (!sort)
        return;
    for (int key = 0; key < NB_SORTS; key++) {
        if (key == SORT_PLAYLIST && id == 0)
            continue;
        if (sort_button(app, &x, key, sort))
            chosen = key;
    }
    if (chosen == -1)
        return;
    sort_set(app->lib, id, chosen);
    db_save_sorts(app->lib);
    library_reorder(app, 0);
}

/*
** text of the value used by the sort (buf holds 32 bytes)
*/
static void sort_value(app_t *app, song_t const *song, int key, char *buf)
{
    time_t date = song->first_seen;

    buf[0] = '\0';
    if (key == SORT_DURATION && song->duration_ms > 0)
        format_time(buf, 32, song->duration_ms);
    if (key == SORT_LISTEN)
        format_time(buf, 32, song_total_listen(app->lib, song));
    if (key == SORT_ADDED && song->first_seen > 0)
        strftime(buf, 32, "%d/%m/%Y", localtime(&date));
    if (key == SORT_RELEASE && song->year)
        snprintf(buf, 32, "%s", song->year);
}

void list_values(app_t *app)
{
    libscene_t *scene = app->libscene;
    view_sort_t *sort = sort_get(app->lib, scene->playlist_id);
    char value[32];
    float pos[3] = {0, 0, 24};

    for (int i = scene->scroll; sort && i < scene->nb_view
        && i < scene->scroll + scene->rows; i++) {
        sort_value(app, scene->view[i], sort->key, value);
        if (value[0] == '\0')
            continue;
        pos[0] = 1800 - widget_text_width(app, value, 24);
        pos[1] = LIST_Y + (i - scene->scroll) * LIST_STEP + 10;
        widget_text(app, value, pos, &gray);
    }
}
