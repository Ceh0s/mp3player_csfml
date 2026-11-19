/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** songs, fingerprints, artists and the library holding them
*/

#ifndef SONG_H_
    #define SONG_H_

    #include "elo.h"

typedef enum role_e {
    ROLE_MAIN,
    ROLE_FEAT,
    ROLE_REMIX,
    NB_ROLES
} role_t;

/*
** id          : internal id, given once, never reused
** merged_into : id of the song this one was merged into (0 if none),
**               its stats are then added to those of that song
** title       : editable title (src_title is what the ID3 tag says)
** artists     : display string, rebuilt from the artist links
** key, hash   : only used while scanning / migrating, never saved here
*/
typedef struct song_s {
    int id;
    int merged_into;
    char *key;
    char *hash;
    char *file;
    char *path;
    char *title;
    char *artists;
    char *album;
    char *genre;
    char *year;
    char *src_title;
    char *src_artists;
    long duration_ms;
    long first_seen;
    long last_seen;
    int present;
    long listen_ms;
    int play_count;
    int skip_count;
    struct song_s *next;
} song_t;

/*
** a fingerprint identifies one audio file; path and file are only known
** if that file is currently in the music folder
*/
typedef struct fprint_s {
    char *hash;
    int song_id;
    char *path;
    char *file;
    struct fprint_s *next;
} fprint_t;

typedef struct artist_s {
    int id;
    char *name;
    struct artist_s *next;
} artist_t;

typedef struct link_s {
    int song_id;
    int artist_id;
    role_t role;
    struct link_s *next;
} link_t;

    #define NAME_LEN 256

/*
** an artist and its role, as edited in the edit screen
*/
typedef struct artist_role_s {
    char name[NAME_LEN];
    role_t role;
} artist_role_t;

typedef struct library_s {
    song_t *songs;
    fprint_t *fprints;
    artist_t *artists;
    link_t *links;
    elo_t *elo;
    song_t **playlist;
    int nb;
    int next_song;
    int next_artist;
    int migrated;
} library_t;

/*
** what an artist did on a song, used for the detail of his stats:
** ORIGINAL : main artist of a song nobody remixed
** REMIXED  : main artist of a song someone else remixed
** REMIXER  : the one who made the remix
** FEAT     : featuring
*/
typedef enum artist_part_e {
    PART_ORIGINAL,
    PART_REMIXED,
    PART_REMIXER,
    PART_FEAT,
    NB_PARTS
} artist_part_t;

typedef struct artist_stats_s {
    long ms[NB_PARTS];
    int titles[NB_PARTS];
    long total_ms;
    int total_titles;
} artist_stats_t;

typedef void (*tsv_fn_t)(library_t *lib, char **fields, int nb);

#endif
