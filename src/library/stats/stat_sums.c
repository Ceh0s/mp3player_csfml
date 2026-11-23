/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** listening time of a song for the statistics screen: its counters
** (all its history, with the songs merged into it) or the sessions of
** listens.log between two dates
*/

#include "mp3player.h"

/*
** the counters of row->song and of the songs merged into it
*/
void stat_totals(library_t *lib, stat_row_t *row)
{
    for (song_t *other = lib->songs; other; other = other->next) {
        if (song_resolve(lib, other) != row->song)
            continue;
        row->ms += other->listen_ms;
        row->plays += other->play_count;
        row->skips += other->skip_count;
    }
}

static int in_range(long date, long const *range)
{
    if (range[0] > 0 && date < range[0])
        return 0;
    return range[1] <= 0 || date < range[1];
}

/*
** by_id : a row per resolved song id (song NULL = not counted); each
** session between range[0] and range[1] (0 = no limit) is added to its
** song
*/
void stat_period(library_t *lib, statscene_t const *scene,
    long const *range, stat_row_t *by_id)
{
    listen_t const *one = NULL;
    song_t *song = NULL;

    for (int i = 0; i < scene->nb_log; i++) {
        one = &scene->log[i];
        if (!in_range(one->date, range))
            continue;
        song = song_resolve(lib, song_by_id(lib, one->song_id));
        if (!song || song->id >= lib->next_song || !by_id[song->id].song)
            continue;
        by_id[song->id].ms += one->ms;
        by_id[song->id].plays++;
        by_id[song->id].skips += one->skip;
    }
}

/*
** "AAAA-MM-JJ" -> its midnight (end = 1 : the midnight after it, to
** count the whole day); returns 0 for an empty text, -1 if it is wrong
*/
long stat_date(char const *text, int end)
{
    struct tm day = {0};
    int year = 0;
    int month = 0;
    int mday = 0;

    if (text[0] == '\0')
        return 0;
    if (sscanf(text, "%d-%d-%d", &year, &month, &mday) != 3
        || year < 1970 || month < 1 || month > 12 || mday < 1 || mday > 31)
        return -1;
    day.tm_year = year - 1900;
    day.tm_mon = month - 1;
    day.tm_mday = mday + end;
    day.tm_isdst = -1;
    return mktime(&day);
}
