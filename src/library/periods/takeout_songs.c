/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reading of a playlist file exported by Google Takeout (csv): one line
** per video, its id then the date it was added to the playlist; and the
** song of a video of this file
*/

#include "mp3player.h"

#include <ctype.h>

/*
** "2018-04-24T09:05:28+00:00" (or with a space, a fraction of second,
** "Z", "UTC") -> seconds, 0 if it is not a date
*/
static long iso_date(char const *text)
{
    struct tm day = {0};
    int hours = 0;
    int minutes = 0;
    int read = 0;
    char sign = 0;

    if (sscanf(text, "%d-%d-%d%*c%d:%d:%d%n", &day.tm_year, &day.tm_mon,
            &day.tm_mday, &day.tm_hour, &day.tm_min, &day.tm_sec,
            &read) != 6)
        return 0;
    day.tm_year -= 1900;
    day.tm_mon--;
    text += read;
    if (*text == '.')
        text++;
    while (isdigit((unsigned char)*text))
        text++;
    if (sscanf(text, " %c%2d:%2d", &sign, &hours, &minutes) < 2
        || (sign != '+' && sign != '-'))
        return timegm(&day);
    minutes += hours * 60;
    return timegm(&day) - (sign == '-' ? -1 : 1) * minutes * 60L;
}

/*
** id (VIDEO_ID bytes) and date of a line; returns 0 for any other line
** (header, in the language of the account, or empty)
*/
int takeout_line(char *line, char *id, long *date)
{
    char *start = line;
    char *comma = NULL;

    if (strncmp(start, "\xEF\xBB\xBF", 3) == 0)
        start += 3;
    while (*start == ' ' || *start == '"')
        start++;
    comma = strchr(start, ',');
    for (int i = 0; i < 11; i++)
        if (!isalnum((unsigned char)start[i]) && start[i] != '_'
            && start[i] != '-')
            return 0;
    if (!comma || comma - start < 11 || comma - start > 12)
        return 0;
    snprintf(id, VIDEO_ID, "%.11s", start);
    comma++;
    while (*comma == ' ' || *comma == '"')
        comma++;
    *date = iso_date(comma);
    return *date > 0;
}

/*
** the song whose file was removed from music/ (edits.log keeps its path,
** "... [id].mp3"): a duplicate merged, or a song deleted
*/
static song_t *removed_song(library_t *lib, char const *id)
{
    FILE *file = fopen(EDITS_LOG, "r");
    char *line = NULL;
    size_t size = 0;
    char *fields[6];
    song_t *song = NULL;

    while (file && getline(&line, &size, file) > 0) {
        if (line[0] == '#' || split_tab(line, fields, 6) < 4
            || strcmp(fields[2], "file") != 0
            || !video_in_name(fields[3], id))
            continue;
        song = song_resolve(lib, song_by_id(lib, atoi(fields[1])));
    }
    free(line);
    if (file)
        fclose(file);
    return song;
}

/*
** the song (resolved) of a video, or of the one played in its place;
** NULL if it is not known
*/
song_t *takeout_song(library_t *lib, char const *id)
{
    video_alias_t const *alias = NULL;
    char const *cur = id;
    song_t *song = song_by_video(lib, id);

    for (int i = 0; i < ALIAS_HOPS && !song; i++) {
        alias = video_alias_find(lib, cur);
        if (!alias)
            break;
        cur = alias->to;
        song = song_by_video(lib, cur);
    }
    return song ? song : removed_song(lib, id);
}
