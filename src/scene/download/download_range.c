/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** duration range of the download screen, kept in settings.tsv: the
** fields are filled with the range typed last when the screen opens,
** and it is saved when the screen is left
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
    settings_t *settings = &app->lib->settings;

    duration_text(app->dlscene->min, settings->dl_min_s);
    duration_text(app->dlscene->max, settings->dl_max_s);
}

/*
** written only if it changed
*/
void dlscene_range_save(app_t *app)
{
    settings_t *settings = &app->lib->settings;
    int min = parse_duration(app->dlscene->min);
    int max = parse_duration(app->dlscene->max);

    if (min == settings->dl_min_s && max == settings->dl_max_s)
        return;
    settings->dl_min_s = min;
    settings->dl_max_s = max;
    db_save_settings(app->lib);
}
