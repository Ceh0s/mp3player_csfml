/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** link between an address and a single chosen playlist: the address can
** be remembered ("Synchroniser" in the library downloads it again), and
** the songs no longer at the address can be reported
*/

#include "mp3player.h"

/*
** opened by "Synchroniser": the options linked to the playlist are
** filled in and the download starts at once
*/
void dlscene_sync(app_t *app)
{
    dlscene_t *scene = app->dlscene;
    source_t *src = source_get(app->lib, app->sync_playlist);

    app->sync_playlist = 0;
    if (!src)
        return;
    snprintf(scene->url, URLS_LEN, "%s", src->url);
    duration_text(scene->min, src->min_s);
    duration_text(scene->max, src->max_s);
    scene->targets[0] = src->playlist_id;
    scene->nb_targets = 1;
    scene->link = 1;
    scene->absent = src->absent;
    dlscene_start(app);
}

/*
** only shown when a single playlist is chosen (and they only apply to a
** single address)
*/
void dlscene_link(app_t *app, float y)
{
    dlscene_t *scene = app->dlscene;
    float link[4] = {130, y, 560, 52};
    float absent[4] = {710, y, 560, 52};

    if (scene->nb_targets != 1)
        return;
    if (widget_button(app, link, scene->link ? "Lier l'adresse à la "
            "playlist : oui" : "Lier l'adresse à la playlist : non"))
        scene->link = !scene->link;
    widget_help(app, link, "Pour la resynchroniser depuis la bibliothèque");
    if (widget_button(app, absent, scene->absent ? "Signaler les titres "
            "retirés : oui" : "Signaler les titres retirés : non"))
        scene->absent = !scene->absent;
}
