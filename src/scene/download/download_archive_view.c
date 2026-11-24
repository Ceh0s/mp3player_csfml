/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** shared by the pages "Refusés", "Exclus" and "Indisponibles" of the
** list over the
** download screen: "Vider la liste" archives the rows shown (asked a
** second time; they stay refused, excluded or unavailable), "Voir les
** archivés"
** shows them again, where they can still be taken back (or forgotten)
*/

#include "mp3player.h"

/*
** 1 if the row is on the screen: in the list shown (archived or not),
** matching the search and in the scroll; i counts the rows matching
*/
int dlpopup_shown(app_t *app, rejected_t const *item, int *i)
{
    int first = app->dlscene->scroll;

    if (item->archived != app->dlscene->archived
        || !search_text(app, item->title))
        return 0;
    (*i)++;
    return *i >= first && *i - first < dlpopup_rows(app);
}

/*
** the rows shown (search included) are archived, then saved
*/
static void archive_all(app_t *app)
{
    dl_popup_t popup = app->dlscene->popup;
    rejected_t *list = app->lib->excluded;

    if (popup == DL_POPUP_REFUSED)
        list = app->lib->rejected;
    if (popup == DL_POPUP_UNAVAILABLE)
        list = app->lib->unavailable;
    for (rejected_t *item = list; item; item = item->next)
        if (search_text(app, item->title))
            item->archived = 1;
    if (popup == DL_POPUP_REFUSED)
        db_save_rejected(app->lib);
    if (popup == DL_POPUP_EXCLUDED)
        db_save_excluded(app->lib);
    if (popup == DL_POPUP_UNAVAILABLE)
        db_save_unavailable(app->lib);
    app->dlscene->scroll = 0;
}

void dlscene_archive_footer(app_t *app, float y)
{
    dlscene_t *scene = app->dlscene;
    float clear[4] = {650, y, 300, 52};

    if (widget_button(app, (float [4]){300, y, 330, 52}, scene->archived
            ? "Voir la liste" : "Voir les archivés")) {
        scene->archived = !scene->archived;
        scene->scroll = 0;
        scene->clear = 0;
        scene->forget = 0;
    }
    if (scene->archived)
        return;
    widget_help(app, clear, "Range les lignes affichées dans les "
        "archivés : elles restent refusées ou exclues");
    if (!widget_button(app, clear, scene->clear ? "Confirmer ?"
            : "Vider la liste"))
        return;
    if (scene->clear)
        archive_all(app);
    scene->clear = !scene->clear;
}
