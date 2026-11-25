/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** sort bar of the library, under the tabs: a button opening the list of
** the criteria, an icon for the direction (clicking the chosen
** criterion again changes it too), and the value of the criterion
** written at the right of each song
*/

#include "mp3player.h"

static const sfColor gray = {190, 190, 190, 255};

/*
** one criterion of the open list; returns 1 if it was chosen
*/
static int criterion(app_t *app, int key, int row, view_sort_t const *sort)
{
    char label[64];
    float rect[4] = {LIST_X, SORT_Y + 60 + row * 52, 400, 50};

    snprintf(label, sizeof(label), (int)sort->key == key ? "> %s <" : "%s",
        sort_def(key)->label);
    return widget_button(app, rect, label);
}

/*
** the list of the criteria, under the sort button; "Tous les titres" is
** not a playlist: it has no order of its own, nor dates of addition
*/
static int criteria(app_t *app, int id, view_sort_t const *sort)
{
    int row = 0;
    int chosen = -1;
    int nb = id == 0 ? NB_SORTS - 2 : NB_SORTS;
    sfColor back = {20, 16, 36, 255};

    widget_rect(app, (float [4]){LIST_X - 6, SORT_Y + 54, 412, nb * 52 + 10},
        &back);
    for (int key = 0; key < NB_SORTS; key++) {
        if ((key == SORT_PLAYLIST || key == SORT_DATE) && id == 0)
            continue;
        if (criterion(app, key, row, sort))
            chosen = key;
        row++;
    }
    return chosen;
}

/*
** clicking the direction icon is like choosing the criterion again
*/
static int direction(app_t *app, view_sort_t const *sort)
{
    float rect[4] = {LIST_X + 412, SORT_Y, 52, 52};

    if (sort->key == SORT_CUSTOM)
        return 0;
    widget_help(app, rect, sort->desc ? "Décroissant" : "Croissant");
    return widget_icon(app, rect, sort->desc ? ICON_DESC : ICON_ASC,
        ICON_OUTLINE);
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
** a click anywhere else closes the list of the criteria
*/
static int choose(app_t *app, int id, view_sort_t const *sort)
{
    libscene_t *scene = app->libscene;
    char label[64];
    int chosen = scene->sort_open ? criteria(app, id, sort) : -1;

    snprintf(label, sizeof(label), "Tri : %s", sort_def(sort->key)->label);
    if (widget_icon_button(app, (float [4]){LIST_X, SORT_Y, 400, 52},
            ICON_LIST, label)) {
        scene->sort_open = !scene->sort_open;
        return -1;
    }
    if (app->wg->mode == WG_CLICK || chosen != -1)
        scene->sort_open = 0;
    if (chosen == -1 && direction(app, sort))
        chosen = sort->key;
    return chosen;
}

/*
** the tab shown is played from its first song, or in a random order
*/
static void launch(app_t *app)
{
    float play[4] = {LIST_X + 480, SORT_Y, 190, 52};
    float random[4] = {LIST_X + 690, SORT_Y, 230, 52};
    libscene_t *scene = app->libscene;

    widget_help(app, play, "Lit l'onglet affiché depuis son premier titre "
        "(aléatoire désactivé)");
    widget_help(app, random, "Lit l'onglet affiché dans un ordre au hasard "
        "(aléatoire activé)");
    if (widget_icon_button(app, play, ICON_PLAY, "Lire"))
        player_start(app, scene->playlist_id, app->search.text, 0);
    if (widget_icon_button(app, random, ICON_SHUFFLE, "Aléatoire"))
        player_start(app, scene->playlist_id, app->search.text, 1);
}

void library_sort_bar(app_t *app)
{
    int id = app->libscene->playlist_id;
    view_sort_t *sort = sort_get(app->lib, id);
    int chosen = -1;

    if (!sort)
        return;
    launch(app);
    chosen = choose(app, id, sort);
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
    period_t *period = NULL;

    buf[0] = '\0';
    if (key == SORT_PLAYLIST || key == SORT_DATE)
        period = period_current(app->lib, app->libscene->playlist_id, song);
    if (period && period->start > 0)
        period_date(buf, period->start);
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
        pos[1] = LIST_Y + (i - scene->scroll) * LIST_STEP + 18;
        widget_text(app, value, pos, &gray);
    }
}
