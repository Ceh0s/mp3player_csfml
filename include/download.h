/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** download of the audio of a video or a sound (YouTube, SoundCloud...)
** by the external program yt-dlp, followed at each tick
*/

#ifndef DOWNLOAD_H_
    #define DOWNLOAD_H_

    #include <sys/types.h>

    #define DL_LINE 256

typedef enum dl_state_e {
    DL_IDLE,
    DL_RUNNING,
    DL_DONE,
    DL_FAILED
} dl_state_t;

/*
** pid / fd : yt-dlp and the read end of the pipe of its output
** line : line of its output being read, last : the last complete one
** (shown as the status, or as the reason of a failure)
*/
typedef struct download_s {
    pid_t pid;
    int fd;
    dl_state_t state;
    float percent;
    char line[DL_LINE];
    int len;
    char last[DL_LINE];
} download_t;

#endif
