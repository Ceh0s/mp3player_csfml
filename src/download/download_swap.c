/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** videos replaced by YouTube Music: a playlist keeps the id of a video
** even when YouTube no longer serves it, and the site then plays another
** version of the same song in its place. yt-dlp only lists the old id;
** the page of the playlist on YouTube Music gives both. Once a playlist
** is listed, its pages are read (by curl, an external program like
** yt-dlp, 100 videos per page) and each video replaced takes the id of
** the one played (swap_parse.c). If anything fails, nothing is replaced.
*/

#include "mp3player.h"
#include <ctype.h>

static int id_char(char c)
{
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

/*
** id of the playlist written in its address (after "list="); 0 if none
*/
static int playlist_id(char const *url, char *id, size_t size)
{
    char const *start = strstr(url, "list=");
    size_t len = 0;

    if (!start)
        return 0;
    start += strlen("list=");
    while (id_char(start[len]) && len < size - 1) {
        id[len] = start[len];
        len++;
    }
    id[len] = '\0';
    return len > 0;
}

/*
** in the child: asks the first page of the playlist, or the next one
** (its token); the answer is written in DL_SWAP_FILE
*/
void swap_exec(download_t const *dl, char const *url)
{
    char body[DL_TOKEN + 256];
    char id[DL_LIST_ID];
    char *const args[] = {"curl", "-s", "--max-time", "20", "-o",
        DL_SWAP_FILE, "-H", "Content-Type: application/json", "-H",
        "Origin: " DL_YTM_SITE, "--data", body, DL_YTM_API, NULL};

    if (dl->page > 0)
        snprintf(body, sizeof(body), DL_YTM_CONTEXT
            "\"continuation\":\"%s\"}", dl->token);
    else if (playlist_id(url, id, sizeof(id)))
        snprintf(body, sizeof(body), DL_YTM_CONTEXT
            "\"browseId\":\"VL%s\"}", id);
    else
        return;
    execvp(args[0], args);
}

/*
** the list is ready: the songs no longer at the address are looked for
** (if the listing worked), then each video is sorted
*/
static void download_listed(app_t *app, int ok)
{
    download_t *dl = &app->download;

    if (ok && dl->found > 0)
        download_find_absent(app);
    dl->found = 0;
    download_sort(app);
}

/*
** end of a listing: the pages of a playlist are read first if curl is
** there, else the videos are sorted at once
*/
void download_list_done(app_t *app, int success)
{
    download_t *dl = &app->download;
    char id[DL_LIST_ID];

    if (success && dl->found > 0 && dl->next > 0
        && playlist_id(dl->queue[dl->next - 1], id, sizeof(id))
        && program_found("curl")) {
        dl->page = 0;
        dl->token[0] = '\0';
        remove(DL_SWAP_FILE);
        if (download_launch(app, dl->queue[dl->next - 1],
                DL_STEP_SWAP) == 0)
            return;
    }
    download_listed(app, success);
}

/*
** a page was read: the next one is asked if there is one
*/
void download_swap_done(app_t *app, int success)
{
    download_t *dl = &app->download;
    int more = success && swap_read(app);

    remove(DL_SWAP_FILE);
    dl->page++;
    if (more && dl->page < DL_SWAP_PAGES
        && download_launch(app, dl->queue[dl->next - 1],
            DL_STEP_SWAP) == 0)
        return;
    download_listed(app, 1);
}

/*
** detail of the report for a video added or already present
*/
char const *swap_detail(dl_entry_t const *entry)
{
    return entry->swapped ? "version jouée par YouTube Music" : "";
}
