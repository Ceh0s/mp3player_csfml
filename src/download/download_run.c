/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** launch of yt-dlp, to list the videos of a link or to download one of
** them, and cancel: it runs in a child process whose output goes in a
** pipe read at each tick (download_poll.c), so the app never waits for
** it. It is an external program, not a library linked to the app: it
** must be installed (sudo pacman -S yt-dlp).
*/

#include "mp3player.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

/*
** one line per video ("id <tab> duration <tab> address <tab> title"),
** without downloading anything; --flat-playlist reads a whole playlist
** in one request
*/
static void exec_list(char const *url, int cookies)
{
    char *const args[] = {"yt-dlp", "--flat-playlist", "--no-playlist",
        "--newline", "-O", DL_FORMAT,
        cookies ? "--cookies-from-browser" : "--no-cookies",
        cookies ? DL_BROWSER : "--no-cookies-from-browser",
        "--", (char *)url, NULL};

    execvp(args[0], args);
}

/*
** The id in the name keeps two videos with the same title apart, and an
** existing file is never replaced. The archive lists the videos already
** in music/ (download_archive.c). The cookies of the browser give access
** to a private video; without them, two neutral options keep the same
** number of arguments.
*/
static void exec_get(char const *url, int cookies)
{
    char *const args[] = {"yt-dlp", "-x", "--audio-format", "mp3",
        "--audio-quality", "0", "--embed-thumbnail", "--embed-metadata",
        "--convert-thumbnails", "jpg", "--no-playlist", "--newline", "-o",
        MUSIC_DIR "/%(title)s [%(id)s].%(ext)s", "--no-overwrites",
        "--no-post-overwrites", "--download-archive", DL_ARCHIVE,
        cookies ? "--cookies-from-browser" : "--no-cookies",
        cookies ? DL_BROWSER : "--no-cookies-from-browser",
        "--", (char *)url, NULL};

    execvp(args[0], args);
}

/*
** in the child: the signals blocked by the app are unblocked, its own
** process group lets the app stop it with ffmpeg (launched by yt-dlp)
*/
static void child_exec(int const *fds, char const *url, download_t *dl)
{
    char const *failed = "yt-dlp n'a pas pu être lancé\n";

    signals_unblock();
    setpgid(0, 0);
    dup2(fds[1], STDOUT_FILENO);
    dup2(fds[1], STDERR_FILENO);
    close(fds[0]);
    close(fds[1]);
    if (dl->step == DL_STEP_LIST)
        exec_list(url, dl->cookies);
    else
        exec_get(url, dl->cookies);
    if (write(STDOUT_FILENO, failed, strlen(failed)) < 0)
        _exit(127);
    _exit(127);
}

/*
** yt-dlp is looked for in the folders of PATH before starting, so the
** app does not create a child process for nothing
*/
static int ytdlp_found(void)
{
    char *paths = getenv("PATH") ? strdup(getenv("PATH")) : NULL;
    char file[PATH_LEN];
    int found = 0;

    for (char *dir = paths ? strtok(paths, ":") : NULL; dir && !found;
        dir = strtok(NULL, ":")) {
        snprintf(file, sizeof(file), "%s/yt-dlp", dir);
        found = access(file, X_OK) == 0;
    }
    free(paths);
    return found;
}

static int can_start(download_t *dl, char const *url)
{
    if (dl->state == DL_RUNNING || url[0] == '\0')
        return 0;
    if (ytdlp_found())
        return 1;
    dl->state = DL_FAILED;
    snprintf(dl->last, DL_LINE, "yt-dlp n'est pas installé "
        "(sudo pacman -S yt-dlp)");
    return 0;
}

static void reset(download_t *dl, dl_step_t step)
{
    dl->state = DL_RUNNING;
    dl->step = step;
    dl->percent = 0;
    dl->len = 0;
    dl->line[0] = '\0';
    snprintf(dl->last, DL_LINE, step == DL_STEP_LIST ?
        "Lecture de la liste des vidéos..." : "Démarrage...");
}

/*
** returns 84 if a download is already running or cannot start
*/
int download_launch(app_t *app, char const *url, dl_step_t step)
{
    download_t *dl = &app->download;
    int fds[2];

    if (!can_start(dl, url) || pipe(fds) != 0)
        return 84;
    reset(dl, step);
    dl->pid = fork();
    if (dl->pid < 0) {
        close(fds[0]);
        close(fds[1]);
        dl->state = DL_FAILED;
        return 84;
    }
    if (dl->pid == 0)
        child_exec(fds, url, dl);
    close(fds[1]);
    dl->fd = fds[0];
    fcntl(dl->fd, F_SETFL, O_NONBLOCK);
    return 0;
}
