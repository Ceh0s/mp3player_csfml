/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** dates of the songs of the playlist taken from YouTube Music: a field
** where the path of the playlist file of Google Takeout (csv) is pasted,
** and "Importer les dates" (src/library/periods/takeout.c)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static void result(pledit_t *pl, takeout_t const *out)
{
    size_t size = sizeof(pl->takeout_msg);

    pl->takeout_ask = out->few;
    if (out->lines == 0)
        snprintf(pl->takeout_msg, size, "Aucune date dans ce fichier");
    else if (out->few)
        snprintf(pl->takeout_msg, size, "Seulement %d vidéos sur %d sont "
            "ici : reclique pour importer", out->in_list, out->lines);
    else
        snprintf(pl->takeout_msg, size, "%d dates lues, %d dans la "
            "playlist, %d changées", out->lines, out->in_list,
            out->changed);
}

static void import(app_t *app, pledit_t *pl)
{
    char path[NAME_LEN];
    takeout_t out;

    path_clean(pl->takeout, path);
    if (path[0] == '\0' || takeout_import(app->lib, pl->playlist, path,
            pl->takeout_ask, &out) == 84) {
        pl->takeout_ask = 0;
        snprintf(pl->takeout_msg, sizeof(pl->takeout_msg),
            "Impossible de lire ce fichier");
        return;
    }
    result(pl, &out);
}

void pledit_takeout(app_t *app)
{
    pledit_t *pl = app->pledit;
    float y = 770 + app->extra_h;
    float field[4] = {780, y, 680, 52};
    float button[4] = {1480, y, 300, 52};

    widget_fit(app, pl->takeout_msg[0] != '\0' ? pl->takeout_msg
        : "Dates YouTube Music : chemin du .csv de Takeout (Ctrl+V)",
        (float [4]){130, y + 12, 22, 630}, &gray);
    if (widget_field(app, field, pl->takeout, pl->focus == 2))
        pl->focus = 2;
    if (widget_button(app, button, "Importer les dates"))
        import(app, pl);
}
