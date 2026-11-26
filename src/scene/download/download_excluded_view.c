/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page "Exclus" of the list over the download screen: the videos never
** to download again; "Reprendre" cancels the exclusion, "Oublier"
** (asked a second time) removes everything about the song, the video
** staying excluded (song_forget.c)
*/

#include "mp3player.h"

static const sfColor gray = {150, 150, 150, 255};

/*
** a song merged into by others, or still in music/, cannot be forgotten
*/
static void not_forgettable(app_t *app, float const *rect)
{
    float pos[3] = {rect[0] + 20, rect[1] + 10, 20};

    widget_text(app, "Non oubliable", pos, &gray);
    widget_tip(app, rect, "D'autres musiques y sont fusionnées, ou son "
        "fichier est encore là");
}

/*
** asked a second time; returns 1 once the song is forgotten
*/
static int forget_button(app_t *app, rejected_t *ex, int row, float y)
{
    dlscene_t *scene = app->dlscene;
    song_t *song = song_by_id(app->lib, ex->song_id);
    float rect[4] = {1345, y, 250, 44};
    int asked = scene->forget == row + 1;

    if (song && !song_forgettable(app->lib, song))
        not_forgettable(app, rect);
    if (!song || !song_forgettable(app->lib, song)
        || !widget_button(app, rect, asked ? "Confirmer ?" : "Oublier"))
        return 0;
    scene->forget = asked ? 0 : row + 1;
    if (!asked)
        return 0;
    song_forget(app, song);
    ex->song_id = 0;
    return 1;
}

static int take_back(app_t *app, rejected_t const *ex, float y)
{
    if (!widget_icon_tip(app, (float [4]){1611, y, 44, 44}, ICON_UNDO,
            "Reprendre"))
        return 0;
    excluded_remove(app->lib, ex->id);
    db_save_excluded(app->lib);
    return 1;
}

/*
** the list changes after a decision: the loop stops there; the songs
** of the excluded videos are looked for once
*/
void dlscene_excluded(app_t *app)
{
    int i = -1;
    float y = DLPOPUP_ROWS_Y - DLPOPUP_STEP;

    if (app->lib->excluded && app->lib->excluded->song_id < 0)
        excluded_songs(app->lib);
    for (rejected_t *ex = app->lib->excluded; ex; ex = ex->next) {
        if (!dlpopup_shown(app, ex, &i))
            continue;
        y += DLPOPUP_STEP;
        widget_fit(app, ex->title, (float [4]){300, y + 8, 22, 820},
            &sfWhite);
        if (forget_button(app, ex, i, y) || take_back(app, ex, y))
            return;
    }
}
