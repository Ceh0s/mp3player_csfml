/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reading of a page of a playlist on YouTube Music (answer of its site,
** written by curl). Each video of the page holds the id kept by the
** playlist (target of its like button), then the id of the video the
** site plays (playlistItemData): they differ when the first one is no
** longer served. The page ends with the token of the next one.
*/

#include "mp3player.h"

static char *read_file(char const *path)
{
    FILE *file = fopen(path, "r");
    char *buf = NULL;
    long size = 0;

    if (!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    buf = size > 0 ? malloc(size + 1) : NULL;
    if (buf && fread(buf, 1, size, file) != (size_t)size) {
        free(buf);
        buf = NULL;
    }
    if (buf)
        buf[size] = '\0';
    fclose(file);
    return buf;
}

/*
** a value of the page, up to its closing quote
*/
static void copy_value(char const *src, char *dest, size_t size)
{
    size_t len = 0;

    while (src[len] && src[len] != '"' && len < size - 1) {
        dest[len] = src[len];
        len++;
    }
    dest[len] = '\0';
}

/*
** the old video stays when it is already in music/, or when another
** version of the song is (found by download_reupload.c), unless it is
** the one played by YouTube Music: downloading it would make a duplicate
*/
static int keep_old(app_t *app, dl_entry_t const *entry,
    char const *fresh)
{
    if (song_by_video(app->lib, entry->id))
        return 1;
    return !song_by_video(app->lib, fresh)
        && reupload_match(app->lib, entry) != NULL;
}

/*
** the entry listed under the old id takes the new one, unless the old
** one is kept or the new one is listed too
*/
static void replace(app_t *app, char const *old, char const *fresh)
{
    download_t *dl = &app->download;
    dl_entry_t *entry = entry_find(dl->listed, old);
    size_t size = strlen(DL_WATCH) + strlen(fresh) + 1;
    char *url = NULL;

    if (!entry || fresh[0] == '\0' || strcmp(old, fresh) == 0
        || entry_find(dl->listed, fresh) || keep_old(app, entry, fresh))
        return;
    url = malloc(size);
    if (!url)
        return;
    snprintf(url, size, DL_WATCH "%s", fresh);
    free(entry->url);
    entry->url = url;
    snprintf(entry->id, VIDEO_ID, "%s", fresh);
    entry->swapped = 1;
}

/*
** one video of the page; returns where the next one starts
*/
static char *swap_item(app_t *app, char *like)
{
    char old[VIDEO_ID];
    char fresh[VIDEO_ID];
    char *next = strstr(like + 1, DL_LIKE_KEY);
    char *item = strstr(like, DL_ITEM_KEY);
    char *id = item ? strstr(item, DL_ID_KEY) : NULL;

    if (!id || (next && id > next))
        return next;
    copy_value(like + strlen(DL_LIKE_KEY), old, VIDEO_ID);
    copy_value(id + strlen(DL_ID_KEY), fresh, VIDEO_ID);
    if (old[0] != '\0' && fresh[0] != '\0' && strcmp(old, fresh) != 0)
        video_alias_add(app->lib, old, fresh);
    replace(app, old, fresh);
    return next;
}

/*
** returns 1 if the playlist has a next page (its token is kept)
*/
int swap_read(app_t *app)
{
    char *buf = read_file(DL_SWAP_FILE);
    char *like = buf ? strstr(buf, DL_LIKE_KEY) : NULL;
    char *token = NULL;

    while (like)
        like = swap_item(app, like);
    token = buf ? strstr(buf, DL_TOKEN_KEY) : NULL;
    if (token)
        copy_value(token + strlen(DL_TOKEN_KEY), app->download.token,
            DL_TOKEN);
    free(buf);
    return token != NULL;
}
