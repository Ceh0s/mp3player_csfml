/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page "Indisponibles" of the list over the download screen: the old
** versions of songs uploaded again ("not available"), left out of the
** downloads; "Retenter" lets the next download of their playlist try
** them again
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static int shown(app_t *app, char const *title, int *i)
{
    int first = app->dlscene->scroll;

    if (!search_text(app, title))
        return 0;
    (*i)++;
    return *i >= first && *i - first < dlpopup_rows(app);
}

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
    float y = DLPOPUP_ROWS_Y - DLPOPUP_STEP;

    for (rejected_t *un = lib->unavailable; un; un = un->next) {
        if (!shown(app, un->title, &i))
            continue;
        y += DLPOPUP_STEP;
        row_text(app, un, y);
        if (widget_button(app, (float [4]){1405, y, 250, 44}, "Retenter")) {
            unavailable_remove(lib, un->id);
            db_save_unavailable(lib);
            return;
        }
    }
}

void dlscene_unavailable_footer(app_t *app, float y)
{
    float all[4] = {300, y, 300, 52};

    if (!app->lib->unavailable)
        return;
    widget_help(app, all, "Elles seront essayées au prochain "
        "téléchargement de leur playlist");
    if (!widget_button(app, all, "Tout retenter"))
        return;
    unavailable_remove(app->lib, NULL);
    db_save_unavailable(app->lib);
    app->dlscene->scroll = 0;
}
