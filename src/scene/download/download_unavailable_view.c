/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page "Indisponibles" of the list over the download screen: the old
** versions of songs uploaded again ("not available"), left out of the
** downloads; "Retenter" lets the next download of their playlist try
** them again; "Vider la liste" archives them (download_archive_view.c)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static void row_text(app_t *app, rejected_t const *un, float y)
{
    char cut[64 + 4];
    char reason[DL_LINE];

    utf8_cut(cut, un->title, 48);
    widget_text(app, cut, (float [3]){300, y + 8, 22}, &sfWhite);
    dl_reason(reason, sizeof(reason), un->reason);
    utf8_cut(cut, reason, 46);
    widget_text(app, cut, (float [3]){900, y + 8, 20}, &gray);
    widget_tip(app, (float [4]){300, y, 1080, DLPOPUP_STEP - 4},
        un->reason);
}

/*
** the list changes after a decision: the loop stops there
*/
void dlscene_unavailable(app_t *app)
{
    library_t *lib = app->lib;
    int i = -1;
    float y = 0;

    for (rejected_t *un = lib->unavailable; un; un = un->next) {
        if (!dlpopup_shown(app, un, &i, &y))
            continue;
        row_text(app, un, y);
        if (widget_icon_tip(app, (float [4]){1611, y, 44, 44}, ICON_SYNC,
                "Retenter")) {
            unavailable_remove(lib, un->id);
            db_save_unavailable(lib);
            return;
        }
    }
}

/*
** only the rows of the list shown (not the archived ones) are retried
*/
static void retry_all(app_t *app)
{
    rejected_t *next = NULL;

    for (rejected_t *un = app->lib->unavailable; un; un = next) {
        next = un->next;
        if (!un->archived && search_text(app, un->title))
            unavailable_remove(app->lib, un->id);
    }
    db_save_unavailable(app->lib);
    scroll_jump(&app->dlscene->list, 0);
}

void dlscene_unavailable_footer(app_t *app, float y)
{
    float all[4] = {970, y, 300, 52};

    if (app->dlscene->archived || unavailable_count(app->lib, 0) == 0)
        return;
    widget_help(app, all, "Au prochain téléchargement");
    if (widget_button(app, all, "Tout retenter"))
        retry_all(app);
}
