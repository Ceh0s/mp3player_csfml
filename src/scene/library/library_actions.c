/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** window of the actions on the songs selected in the library (called
** to draw it and to handle a click, see widgets.c): opinion, playlist,
** title, image, exclusion and removal (both asked twice)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

/*
** the next playlist that can be edited by hand (step = 1 or -1)
*/
static void next_playlist(app_t *app, int step)
{
    selection_t *sel = &app->libscene->sel;
    playlist_t *pl = NULL;
    int count = 0;
    playlist_t *list[64];
    int at = 0;

    for (pl = app->lib->playlists; pl && count < 64; pl = pl->next)
        if (pl->auto_like == 0) {
            at = pl->id == sel->playlist ? count : at;
            list[count] = pl;
            count++;
        }
    if (count == 0)
        return;
    sel->playlist = list[(at + step + count) % count]->id;
}

static void label(app_t *app, char const *text, float y)
{
    widget_text(app, text, (float [3]){300, y + 12, 26}, &gray);
}

static void opinion_row(app_t *app, float y)
{
    label(app, "Avis", y);
    if (widget_button(app, (float [4]){520, y, 200, 52}, "J'aime"))
        bulk_like(app, 1);
    if (widget_button(app, (float [4]){735, y, 240, 52}, "Je n'aime pas"))
        bulk_like(app, -1);
    if (widget_button(app, (float [4]){990, y, 220, 52}, "Sans avis"))
        bulk_like(app, 0);
    if (widget_button(app, (float [4]){1225, y, 430, 52},
            "Appliquer les suggestions"))
        bulk_suggest(app);
}

/*
** the playlist chosen, the first one that can be edited if none
*/
static playlist_t *chosen(app_t *app)
{
    selection_t *sel = &app->libscene->sel;
    playlist_t *pl = playlist_by_id(app->lib, sel->playlist);

    if (!pl || pl->auto_like != 0)
        next_playlist(app, 1);
    return playlist_by_id(app->lib, sel->playlist);
}

static void playlist_row(app_t *app, float y)
{
    playlist_t *pl = chosen(app);
    char name[64];

    label(app, "Playlist", y);
    if (!pl || pl->auto_like != 0) {
        widget_text(app, "Aucune playlist", (float [3]){520, y + 12, 24},
            &gray);
        return;
    }
    if (widget_button(app, (float [4]){520, y, 52, 52}, "<"))
        next_playlist(app, -1);
    utf8_cut(name, pl->name, 24);
    widget_text(app, name, (float [3]){590, y + 12, 26}, &sfWhite);
    if (widget_button(app, (float [4]){960, y, 52, 52}, ">"))
        next_playlist(app, 1);
    if (widget_button(app, (float [4]){1030, y, 200, 52}, "Ajouter"))
        bulk_playlist(app, 1);
    if (widget_button(app, (float [4]){1245, y, 200, 52}, "Retirer"))
        bulk_playlist(app, 0);
}

/*
** the dangerous actions are asked twice: labels = {first, second}
*/
static void confirm_button(app_t *app, float const *rect, int action,
    char const *const *labels)
{
    selection_t *sel = &app->libscene->sel;
    int asked = sel->confirm == action;

    if (!widget_button(app, rect, labels[asked]))
        return;
    sel->confirm = asked ? 0 : action;
    if (asked)
        bulk_remove(app, action == SEL_EXCLUDE);
}

static void remove_row(app_t *app, float y)
{
    float exclude[4] = {520, y, 300, 52};
    float delete[4] = {835, y, 420, 52};

    label(app, "Fichiers", y);
    widget_help(app, exclude, "Ne seront plus jamais téléchargés");
    widget_help(app, delete, "Les stats sont gardées");
    confirm_button(app, exclude, SEL_EXCLUDE,
        (char const *[2]){"Exclure", "Confirmer"});
    confirm_button(app, delete, SEL_DELETE,
        (char const *[2]){"Supprimer du lecteur", "Confirmer la suppression"});
}

static void window(app_t *app)
{
    sfFloatRect view = app->view_rect;
    float all[4] = {view.left, view.top, view.width, view.height};
    float back[4] = {260, 140, 1420, 730 + app->extra_h};
    sfColor shade = {0, 0, 0, 170};
    sfColor dark = {30, 30, 30, 245};

    widget_rect(app, all, &shade);
    widget_rect(app, back, &dark);
}

void library_actions(app_t *app)
{
    selection_t *sel = &app->libscene->sel;
    char title[64];

    window(app);
    snprintf(title, sizeof(title), "%d titre(s) sélectionné(s)", sel->nb);
    widget_text(app, title, (float [3]){300, 165, 34}, &sfWhite);
    widget_text(app, sel->msg, (float [3]){300, 220, 22}, &gray);
    opinion_row(app, 270);
    playlist_row(app, 345);
    library_insert_rows(app, 420);
    library_image_row(app, 590);
    remove_row(app, 665);
    if (widget_button(app, (float [4]){1455, 790 + app->extra_h, 200, 52},
            "Fermer"))
        sel->open = 0;
}
