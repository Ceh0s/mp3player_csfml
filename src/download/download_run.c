/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** start and cancel of a download: yt-dlp runs in a child process whose
** output goes in a pipe read at each tick (download_poll.c), so the app
** never waits for it. It is an external program, not a library linked
** to the app: it must be installed (sudo pacman -S yt-dlp).
*/

#include "mp3player.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

/*
** in the child: the signals blocked by the app are unblocked, its own
** process group lets the app stop it with ffmpeg (launched by yt-dlp)
*/
static void child_exec(int const *fds, char const *url)
{
    char *const args[] = {"yt-dlp", "-x", "--audio-format", "mp3",
        "--audio-quality", "0", "--embed-thumbnail", "--embed-metadata",
        "--convert-thumbnails", "jpg", "--no-playlist", "--newline", "-o",
        MUSIC_DIR "/%(title)s.%(ext)s", "--", (char *)url, NULL};
    char const *failed = "yt-dlp n'a pas pu être lancé\n";

    signals_unblock();
    setpgid(0, 0);
    dup2(fds[1], STDOUT_FILENO);
    dup2(fds[1], STDERR_FILENO);
    close(fds[0]);
    close(fds[1]);
    execvp(args[0], args);
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

static void reset(download_t *dl)
{
    dl->state = DL_RUNNING;
    dl->percent = 0;
    dl->len = 0;
    dl->line[0] = '\0';
    snprintf(dl->last, DL_LINE, "Démarrage...");
}

/*
** returns 84 if a download is already running or cannot start
*/
int download_start(app_t *app, char const *url)
{
    download_t *dl = &app->download;
    int fds[2];

    if (!can_start(dl, url) || pipe(fds) != 0)
        return 84;
    dl->pid = fork();
    if (dl->pid < 0) {
        close(fds[0]);
        close(fds[1]);
        return 84;
    }
    if (dl->pid == 0)
        child_exec(fds, url);
    close(fds[1]);
    dl->fd = fds[0];
    fcntl(dl->fd, F_SETFL, O_NONBLOCK);
    reset(dl);
    return 0;
}

/*
** stops yt-dlp and ffmpeg (its whole process group) and waits for it,
** so no process is left behind (also called when the app closes)
*/
void download_cancel(app_t *app)
{
    download_t *dl = &app->download;

    if (dl->state != DL_RUNNING)
        return;
    kill(-dl->pid, SIGTERM);
    waitpid(dl->pid, NULL, 0);
    close(dl->fd);
    dl->state = DL_FAILED;
    snprintf(dl->last, DL_LINE, "Téléchargement annulé");
}
