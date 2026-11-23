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
    #include "song.h"

    #define DL_LINE 512
    #define DL_TARGETS 16
    #define DL_BROWSER "firefox"
    #define DL_ARCHIVE "data/downloads.archive"
    #define DL_FORMAT_1 "%(id)s\t%(duration)s\t%(live_status)s"
    #define DL_FORMAT DL_FORMAT_1 "\t%(webpage_url)s\t%(title)s"

typedef enum dl_state_e {
    DL_IDLE,
    DL_RUNNING,
    DL_DONE,
    DL_FAILED
} dl_state_t;

/*
** a link is first listed (LIST: yt-dlp only writes the videos it holds,
** with their duration), then each video kept is downloaded (GET)
*/
typedef enum dl_step_e {
    DL_STEP_LIST,
    DL_STEP_GET
} dl_step_t;

/*
** what the download screen chose for a queue, copied when it starts
** (changing the screen then does not change the running queue)
** targets : ids of the local playlists the songs are added to
** min_s / max_s : allowed duration in seconds (-1 = no limit)
** link : 1 to link the address to the playlist (one link, one playlist)
** absent : 1 to report the songs of the playlist no longer at the
** address (one link, one playlist)
*/
typedef struct dl_options_s {
    int targets[DL_TARGETS];
    int nb_targets;
    int min_s;
    int max_s;
    int link;
    int absent;
} dl_options_t;

/*
** a video found by the listing: id given by the site, address of its
** page, title, duration in seconds (-1 if unknown), live = 1 for a live
** stream (never ending: not downloaded) and the playlists it goes to;
** was_refused = 1 if it was refused before for its duration and is now
** in the allowed one (asked again instead of downloaded)
*/
typedef struct dl_entry_s {
    char id[VIDEO_ID];
    char *url;
    char *title;
    int duration;
    int live;
    int was_refused;
    int targets[DL_TARGETS];
    int nb_targets;
    struct dl_entry_s *next;
} dl_entry_t;

/*
** what became of a video (or of a link whose listing failed), for the
** report shown at the end of a queue or when it is stopped
*/
typedef enum report_status_e {
    REP_ADDED,
    REP_PRESENT,
    REP_EXCLUDED,
    REP_REFUSED,
    REP_FLAGGED,
    REP_UNAVAILABLE,
    REP_FAILED,
    REP_INTERRUPTED,
    NB_REPORTS
} report_status_t;

/*
** one line of the report: title of the video (or address of the link)
** and a detail (reason of a failure, duration...)
*/
typedef struct dl_report_s {
    report_status_t status;
    char *title;
    char *detail;
    struct dl_report_s *next;
} dl_report_t;

/*
** measures used to estimate the whole download; an element is a video
** elapsed_us : time spent since the start of the queue
** done : elements ended, bytes : their size, sized : how many of them
** had a known size
** cur_bytes / speed / cur_eta : size, speed (bytes per second) and time
** left (seconds, -1 if unknown) of the element being downloaded
*/
typedef struct dl_stats_s {
    long elapsed_us;
    int done;
    double bytes;
    int sized;
    double cur_bytes;
    double speed;
    long cur_eta;
} dl_stats_t;

/*
** pid / fd : yt-dlp and the read end of the pipe of its output
** step : what yt-dlp is doing (listing a link or getting a video)
** line : line of its output being read, last : the last complete one
** (shown as the status, or as the reason of a failure)
** queue : the links still to list (NULL when nothing runs), next :
** index of the next one, total : their number, started : date (seconds)
** the queue started
** listed : videos of the link being listed, todo : videos still to
** download, cur : the one being downloaded, flagged : videos out of the
** allowed duration, waiting for a decision (kept after the queue)
** found : videos listed for the link being listed
** item / items : position of cur among the videos to download
** ok / failed : videos downloaded or failed (a failed listing counts
** too), skipped : already in music/ (only added to the playlists),
** refused : out of the duration and already refused, unavailable :
** private or deleted videos, error : the last failure
** absent / nb_absent : songs of the playlist absent_pl no longer at
** the address listed (resolved ids), waiting for a decision
** ask : 1 when the queue ended with videos put aside or songs absent:
** the download screen then opens their list once
** cookies : 1 if yt-dlp reads the cookies of DL_BROWSER (needed for a
** private playlist), kept during the session
** excluded : videos never to download again (excluded.tsv) left out
** report : what became of each video of the queue (kept until the next
** queue), show_report : 1 when the queue ended or was stopped: the
** download screen then opens the report once
*/
typedef struct download_s {
    pid_t pid;
    int fd;
    dl_state_t state;
    dl_step_t step;
    float percent;
    char line[DL_LINE];
    int len;
    char last[DL_LINE];
    char **queue;
    int next;
    int total;
    long started;
    dl_options_t opt;
    dl_entry_t *listed;
    dl_entry_t *todo;
    dl_entry_t *cur;
    dl_entry_t *flagged;
    int found;
    int item;
    int items;
    int ok;
    int failed;
    int skipped;
    int refused;
    int excluded;
    int unavailable;
    char error[DL_LINE];
    int *absent;
    int nb_absent;
    int absent_pl;
    int ask;
    int cookies;
    dl_report_t *report;
    int show_report;
    dl_stats_t stats;
} download_t;

#endif
