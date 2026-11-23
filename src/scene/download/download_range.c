/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** duration range of the download screen, kept until the app closes
** (app->dl_range): the fields are filled with the range typed last when
** the screen opens, and it is kept when the screen is left
*/

#include "mp3player.h"

/*
** seconds written back as "m:ss" in a duration field ("" = no limit)
*/
void duration_text(char *buf, int secs)
{
    buf[0] = '\0';
    if (secs >= 0)
        snprintf(buf, DURATION_LEN, "%d:%02d", secs / 60, secs % 60);
}

void dlscene_range_load(app_t *app)
{
    duration_text(app->dlscene->min, app->dl_range[0]);
    duration_text(app->dlscene->max, app->dl_range[1]);
}

void dlscene_range_save(app_t *app)
{
    app->dl_range[0] = parse_duration(app->dlscene->min);
    app->dl_range[1] = parse_duration(app->dlscene->max);
}
