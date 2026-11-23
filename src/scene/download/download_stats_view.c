/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** size and time of the download screen: the element being downloaded
** (size, speed) and the whole queue (downloaded, estimated total size,
** estimated time left), from download_estimate
*/

#include "mp3player.h"

static const sfColor light = {220, 220, 220, 255};

void format_size(char *buf, size_t size, double bytes)
{
    snprintf(buf, size, "%.1f Mio", bytes / (1024.0 * 1024.0));
}

static void element_line(app_t *app, dl_stats_t const *st)
{
    char size[32];
    char speed[32];
    char line[96];

    if (st->cur_bytes <= 0)
        return;
    format_size(size, sizeof(size), st->cur_bytes);
    if (st->speed > 0) {
        format_size(speed, sizeof(speed), st->speed);
        snprintf(line, sizeof(line), "Élément : %s à %s/s", size, speed);
    } else
        snprintf(line, sizeof(line), "Élément : %s", size);
    widget_text(app, line, (float [3]){130, 675, 22}, &light);
}

/*
** "au moins" while no element is over: only the current one is known
*/
static void time_left(char *buf, size_t size, download_t const *dl,
    long secs)
{
    char time[32];
    int partial = dl->stats.done == 0 && download_rest(dl) > 0;

    if (secs < 0) {
        snprintf(buf, size, "reste : calcul...");
        return;
    }
    format_time(time, sizeof(time), secs * 1000);
    snprintf(buf, size, "reste %s %s", partial ? "au moins" : "environ",
        time);
}

static void total_line(app_t *app, download_t const *dl)
{
    dl_stats_t const *st = &dl->stats;
    double total = 0;
    long secs = download_estimate(dl, &total);
    char got[32];
    char all[32];
    char left[64];
    char line[160];

    format_size(got, sizeof(got), st->bytes
        + st->cur_bytes * dl->percent / 100);
    format_size(all, sizeof(all), total);
    time_left(left, sizeof(left), dl, secs);
    snprintf(line, sizeof(line), "Total : %s sur %s %s, %s", got,
        st->sized == 0 && download_rest(dl) > 0 ? "au moins" : "environ",
        all, left);
    widget_text(app, line, (float [3]){130, 715, 22}, &light);
}

void dlscene_stats(app_t *app, download_t const *dl)
{
    element_line(app, &dl->stats);
    total_line(app, dl);
}
