/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** options of the download screen: allowed duration (two fields, empty =
** no limit) and the keys typed in the fields; dlscene_options gathers
** every option for download_start
*/

#include "mp3player.h"

/*
** "3:30" or "1:02:00" (h:m:s), or a number of minutes alone ("8");
** -1 if empty or not a duration
*/
int parse_duration(char const *text)
{
    int h = 0;
    int m = 0;
    int s = 0;

    if (sscanf(text, "%d:%d:%d", &h, &m, &s) == 3)
        return h * 3600 + m * 60 + s;
    if (sscanf(text, "%d:%d", &m, &s) == 2)
        return m * 60 + s;
    if (sscanf(text, "%d", &m) == 1)
        return m * 60;
    return -1;
}

void dlscene_options(app_t *app, dl_options_t *opt)
{
    dlscene_t *scene = app->dlscene;

    memcpy(opt->targets, scene->targets, sizeof(opt->targets));
    opt->nb_targets = scene->nb_targets;
    opt->min_s = parse_duration(scene->min);
    opt->max_s = parse_duration(scene->max);
    opt->link = scene->link;
    opt->absent = scene->absent;
}

/*
** the duration fields only take digits and ':' (8 = backspace)
*/
void dlscene_text(app_t *app, sfUint32 code)
{
    dlscene_t *scene = app->dlscene;
    char *buf = scene->focus == DL_FIELD_MIN ? scene->min : scene->max;

    if (scene->focus == DL_FIELD_URL) {
        text_input_n(scene->url, URLS_LEN, code);
        return;
    }
    if (code == 8 || code == ':' || (code >= '0' && code <= '9'))
        text_input_n(buf, DURATION_LEN, code);
}

/*
** "Durée : [min] à [max]"
*/
void dlscene_durations(app_t *app, float y)
{
    dlscene_t *scene = app->dlscene;
    float min[4] = {1000, y, 150, 56};
    float max[4] = {1210, y, 150, 56};

    widget_text(app, "Durée :", (float [3]){900, y + 12, 24}, &sfWhite);
    if (widget_field(app, min, scene->min, scene->focus == DL_FIELD_MIN))
        scene->focus = DL_FIELD_MIN;
    widget_text(app, "à", (float [3]){1172, y + 12, 24}, &sfWhite);
    if (widget_field(app, max, scene->max, scene->focus == DL_FIELD_MAX))
        scene->focus = DL_FIELD_MAX;
    widget_help(app, min, "Durée minimale (m:ss ou minutes), vide = aucune");
    widget_help(app, max, "Durée maximale (m:ss ou minutes), vide = aucune");
}
