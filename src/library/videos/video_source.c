/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** link between a song and the video it was downloaded from: the address
** written by yt-dlp in the file ("purl" tag), or the id in the name of
** the file ("title [id].mp3")
*/

#include "mp3player.h"

/*
** copies in id (12 bytes) the id of 11 characters of a YouTube address
** (youtube.com or music.youtube.com); returns 0 if there is none
*/
int source_video_id(char const *source, char *id)
{
    char const *start = source ? strstr(source, "watch?v=") : NULL;

    if (!start || strlen(start + 8) < 11)
        return 0;
    snprintf(id, 12, "%.11s", start + 8);
    return 1;
}

/*
** a downloaded file is named "... [id].mp3" (path or name alone)
*/
int video_in_name(char const *path, char const *id)
{
    size_t len = strlen(path);
    size_t id_len = strlen(id);

    if (len < id_len + 7)
        return 0;
    path += len - id_len - 6;
    return path[-1] == '[' && strncmp(path, id, id_len) == 0
        && strcmp(path + id_len, "].mp3") == 0;
}

/*
** the present song holding the video of this id (resolved), or NULL
*/
song_t *song_by_video(library_t *lib, char const *id)
{
    char found[12];

    for (fprint_t *fp = lib->fprints; fp; fp = fp->next) {
        if (!fp->path)
            continue;
        if (video_in_name(fp->path, id) || (source_video_id(fp->source, found)
                && strcmp(found, id) == 0))
            return song_resolve(lib, song_by_id(lib, fp->song_id));
    }
    return same_file_video(lib, id);
}

/*
** id of the video of a file present (VIDEO_ID bytes): from its address,
** or from its name "title [id].mp3"; returns 0 if it has none
*/
int fprint_video_id(fprint_t const *fp, char *id)
{
    char const *path = fp->path;
    char const *start = NULL;
    size_t len = 0;

    if (!path)
        return 0;
    if (source_video_id(fp->source, id))
        return 1;
    len = strlen(path);
    start = strrchr(path, '[');
    if (len < 6 || strcmp(path + len - 5, "].mp3") != 0 || !start)
        return 0;
    len = path + len - 5 - (start + 1);
    if (len == 0 || len >= VIDEO_ID)
        return 0;
    snprintf(id, len + 1, "%s", start + 1);
    return 1;
}

/*
** id (VIDEO_ID bytes) of the video one of the files of the song came
** from; returns 0 if none of them came from a video
*/
int song_video_id(library_t *lib, song_t *song, char *id)
{
    for (fprint_t *fp = lib->fprints; fp; fp = fp->next)
        if (song_resolve(lib, song_by_id(lib, fp->song_id)) == song
            && fprint_video_id(fp, id))
            return 1;
    return 0;
}
