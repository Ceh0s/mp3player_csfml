/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page "Refusés" of the list over the download screen: the videos
** refused for their duration; "Reprendre" cancels the refusal (the
** archived ones are shown with "Voir les archivés")
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

/*
** the list changes after a decision: the loop stops there
*/
void dlscene_refused(app_t *app)
{
    int i = -1;
    float y = 0;
    char text[32];

    for (rejected_t *rej = app->lib->rejected; rej; rej = rej->next) {
        if (!dlpopup_shown(app, rej, &i, &y))
            continue;
        widget_fit(app, rej->title, (float [4]){300, y + 8, 22, 740},
            &sfWhite);
        format_time(text, sizeof(text), rej->duration * 1000L);
        widget_text(app, text, (float [3]){1070, y + 8, 22}, &gray);
        if (widget_icon_tip(app, (float [4]){1611, y, 44, 44}, ICON_UNDO,
                "Reprendre")) {
            rejected_remove(app->lib, rej->id);
            db_save_rejected(app->lib);
            return;
        }
    }
}
