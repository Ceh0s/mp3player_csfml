/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** size and time of the download: read from the progress lines of yt-dlp
** ("[download]  42.3% of ~ 3.52MiB at 1.23MiB/s ETA 00:02"), then the
** whole queue is estimated from the average of the elements already done
** (their time includes the conversion by ffmpeg)
*/

#include "mp3player.h"

static double unit_factor(char const *unit)
{
    if (strncmp(unit, "KiB", 3) == 0)
        return 1024.0;
    if (strncmp(unit, "MiB", 3) == 0)
        return 1024.0 * 1024.0;
    if (strncmp(unit, "GiB", 3) == 0)
        return 1024.0 * 1024.0 * 1024.0;
    return 1.0;
}

/*
** the size written after _key_ (" of ", " at "), in bytes; -1 if it is
** absent or "Unknown" (a ~ means the size is approximate)
*/
static double read_size(char const *line, char const *key)
{
    char const *start = strstr(line, key);
    char *end = NULL;
    double value = 0;

    if (!start)
        return -1;
    start += strlen(key);
    while (*start == ' ' || *start == '~')
        start++;
    value = strtod(start, &end);
    if (end == start)
        return -1;
    return value * unit_factor(end);
}

/*
** "ETA 01:02:03" or "ETA 02:03", in seconds; -1 if unknown
*/
static long read_eta(char const *line)
{
    char const *start = strstr(line, "ETA ");
    int hours = 0;
    int min = 0;
    int sec = 0;

    if (!start)
        return -1;
    if (sscanf(start, "ETA %d:%d:%d", &hours, &min, &sec) == 3)
        return hours * 3600 + min * 60 + sec;
    if (sscanf(start, "ETA %d:%d", &min, &sec) == 2)
        return min * 60 + sec;
    return -1;
}

/*
** only the progress lines are read (they start with a percentage)
*/
void stats_parse(dl_stats_t *st, char const *line)
{
    float percent = 0;
    double size = 0;

    if (sscanf(line, "[download] %f%%", &percent) != 1)
        return;
    size = read_size(line, " of ");
    if (size > 0)
        st->cur_bytes = size;
    st->speed = read_size(line, " at ");
    st->cur_eta = read_eta(line);
}

/*
** the element being downloaded is over (next item of a playlist, or end
** of the link)
*/
void stats_item_done(dl_stats_t *st)
{
    st->done++;
    if (st->cur_bytes > 0) {
        st->bytes += st->cur_bytes;
        st->sized++;
    }
    st->cur_bytes = 0;
    st->speed = -1;
    st->cur_eta = -1;
}

/*
** elements not started yet: the links waiting (a playlist not started
** counts for one, its size is only known when it starts) and the rest
** of the playlist being downloaded
*/
int download_rest(download_t const *dl)
{
    int rest = dl->total - dl->next;

    if (dl->items > 0)
        rest += dl->items - dl->item;
    return rest;
}

/*
** returns the seconds left for the whole queue and puts its total size
** (bytes) in _size_; without any element done, only the current one is
** known: the view then writes "au moins". -1 if nothing is known.
*/
long download_estimate(download_t const *dl, double *size)
{
    dl_stats_t const *st = &dl->stats;
    int rest = download_rest(dl);
    double average = 0;
    long left = st->cur_eta;

    *size = st->bytes + st->cur_bytes;
    if (st->sized > 0)
        *size += st->bytes / st->sized * rest;
    if (st->done == 0)
        return left;
    average = st->elapsed_us / 1000000.0 / st->done;
    if (left < 0)
        left = average * (1 - dl->percent / 100);
    return left + average * rest;
}
