/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** follows the download at each tick: reads what yt-dlp wrote (without
** waiting, the pipe is non blocking), the progress comes from its lines
** "[download]  42.3% of ...", and checks if it ended (waitpid WNOHANG)
*/

#include "mp3player.h"
#include <sys/wait.h>

static void end_line(download_t *dl)
{
    float percent = 0;

    dl->line[dl->len] = '\0';
    dl->len = 0;
    if (dl->line[0] == '\0')
        return;
    if (sscanf(dl->line, "[download] %f%%", &percent) == 1)
        dl->percent = percent;
    snprintf(dl->last, DL_LINE, "%s", dl->line);
}

/*
** yt-dlp may end a line with \r or \n; a too long line is cut
*/
static void add_char(download_t *dl, char c)
{
    if (c == '\n' || c == '\r') {
        end_line(dl);
        return;
    }
    if (dl->len >= DL_LINE - 1)
        return;
    dl->line[dl->len] = c;
    dl->len++;
}

static void read_output(download_t *dl)
{
    char buf[512];
    ssize_t size = read(dl->fd, buf, sizeof(buf));

    while (size > 0) {
        for (ssize_t i = 0; i < size; i++)
            add_char(dl, buf[i]);
        size = read(dl->fd, buf, sizeof(buf));
    }
}

/*
** the new mp3 is added to the library (and to the list shown)
*/
static void finish(app_t *app, int status)
{
    download_t *dl = &app->download;

    read_output(dl);
    end_line(dl);
    close(dl->fd);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        dl->state = DL_FAILED;
        return;
    }
    dl->state = DL_DONE;
    dl->percent = 100;
    library_rescan(app);
    if (app->libscene)
        list_create(app);
}

void download_poll(app_t *app)
{
    download_t *dl = &app->download;
    int status = 0;

    if (dl->state != DL_RUNNING)
        return;
    read_output(dl);
    if (waitpid(dl->pid, &status, WNOHANG) == dl->pid)
        finish(app, status);
}
