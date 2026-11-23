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
** source      : address the file was downloaded from (TXXX "purl"
**               frame written by yt-dlp), only while scanning
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
    char *source;
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
** a fingerprint identifies one audio file; path, file and source (the
** address it was downloaded from) are only known if that file is
** currently in the music folder
*/
typedef struct fprint_s {
    char *hash;
    int song_id;
    char *path;
    char *file;
    char *source;
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

/*
** songs : raw ids (resolved when used), in the order of the playlist
** auto_like : 1 for "Titres likés", -1 for "Titres dislikés" (made from
** the likes, not editable), 0 for a playlist made by hand
*/
typedef struct playlist_s {
    int id;
    char *name;
    int *songs;
    int nb;
    int auto_like;
    struct playlist_s *next;
} playlist_t;

/*
** like (value 1) or dislike (-1) of a song (raw id) or an artist
*/
typedef struct like_s {
    int kind;
    int id;
    int value;
    struct like_s *next;
} like_t;

typedef enum rule_type_e {
    RULE_ALL,
    RULE_PLAYLIST,
    RULE_ARTIST,
    RULE_SONG,
    NB_RULE_TYPES
} rule_type_t;

/*
** a rule of a ranking: include (1) or exclude (0) all the songs, those
** of a playlist, of an artist, or one song (target = its raw id)
*/
typedef struct rule_s {
    int include;
    rule_type_t type;
    int target;
    struct rule_s *next;
} rule_t;

/*
** a ranking (board) has its own Elo ratings; a present song is in it
** if a rule includes it and no rule excludes it. The general one (id 0)
** holds every song, it is never saved nor modified.
*/
typedef struct board_s {
    int id;
    char *name;
    rule_t *rules;
    struct board_s *next;
} board_t;

/*
** criteria to sort the songs of a playlist (or of "Tous les titres"):
** PLAYLIST is the order of the playlist, which is the order in which the
** songs were added; CUSTOM is the order chosen by moving the songs
*/
typedef enum sort_key_e {
    SORT_PLAYLIST,
    SORT_CUSTOM,
    SORT_TITLE,
    SORT_ARTIST,
    SORT_DURATION,
    SORT_LISTEN,
    SORT_ADDED,
    SORT_RELEASE,
    NB_SORTS
} sort_key_t;

/*
** name : saved in playlist_sorts.tsv, label : shown on its button,
** desc_first : direction when it is chosen (1 = biggest first)
*/
typedef struct sort_def_s {
    char const *name;
    char const *label;
    int desc_first;
} sort_def_t;

/*
** sort of one playlist (id 0 = "Tous les titres"): key, direction and
** the custom order (raw song ids, saved apart in custom_orders.tsv)
*/
typedef struct view_sort_s {
    int playlist_id;
    sort_key_t key;
    int desc;
    int *custom;
    int nb_custom;
    struct view_sort_s *next;
} view_sort_t;

/*
** a song being sorted, with its value for the chosen key: a text (title,
** artists) or a number; known = 0 for a missing value (always last)
*/
typedef struct sort_item_s {
    song_t *song;
    char const *text;
    long num;
    int known;
} sort_item_t;

    #define VIDEO_ID 64
    #define COVER_SIG 16
    #define SUGGEST_MAX 12
    #define SUG_VISIBLE 0
    #define SUG_HIDDEN 1
    #define SUG_DELETED 2

/*
** a video refused in the download screen because of its duration: it
** is left out again while its duration stays out of the allowed one
** (saved in rejected.tsv)
*/
typedef struct rejected_s {
    char id[VIDEO_ID];
    char *title;
    int duration;
    long date;
    struct rejected_s *next;
} rejected_t;

/*
** address of a playlist of a site (YouTube...) linked to a local
** playlist: "Synchroniser" downloads it again with the same allowed
** duration (seconds, -1 = no limit); absent = 1 to report the songs no
** longer in it (saved in playlist_sources.tsv)
*/
typedef struct source_s {
    int playlist_id;
    char *url;
    int min_s;
    int max_s;
    int absent;
    struct source_s *next;
} source_t;

/*
** sound envelope of an audio file (loudness every ENV_FRAME_MS, one byte
** each), used to tell if two files hold the same recording; nb = 0 when
** the file could not be read. Kept by fingerprint in envelopes.tsv.
*/
    #define ENV_FRAME_MS 100
    #define ENV_MAX 6000

typedef struct envelope_s {
    char *hash;
    unsigned char *data;
    int nb;
    struct envelope_s *next;
} envelope_t;

/*
** two songs marked "Pas un doublon" (raw ids, saved in
** not_duplicates.tsv)
*/
typedef struct not_dup_s {
    int a;
    int b;
    struct not_dup_s *next;
} not_dup_t;

/*
** what is compared to find duplicates, computed once per present song:
** core : title without the artist before " - ", the brackets, the
** featurings and the words like "official video" (dup_clean.c)
** artists : "|a|b|", every name known for the song: its artists, its
** tag (the uploader of a video), the artist written before " - "
** version : one bit per version word (remix, live, slowed...)
*/
typedef struct dup_info_s {
    song_t *song;
    char core[NAME_LEN];
    char artists[NAME_LEN * 2];
    int version;
} dup_info_t;

/*
** a possible duplicate; audio : likeness of the sound (0 to 100), -1 if
** it could not be compared
*/
typedef struct dup_pair_s {
    song_t *a;
    song_t *b;
    int audio;
} dup_pair_t;

/*
** pairs found by dup_find; since > 0 only keeps the pairs with a song
** seen since then (just downloaded)
*/
typedef struct dup_list_s {
    dup_pair_t *pairs;
    int nb;
    long since;
} dup_list_t;

/*
** order between the ex aequo of a ranking (board) for a kind (songs or
** artists): raw ids, the first one comes first
*/
typedef struct tie_order_s {
    int board;
    int kind;
    int *ids;
    int nb;
    struct tie_order_s *next;
} tie_order_t;

/*
** title suggestions (src/library/suggest): each rule proposes one
** change only, accepted, hidden or deleted on its own
*/
typedef enum suggest_rule_e {
    SUG_SPACES,
    SUG_QUOTES,
    SUG_HASHTAGS,
    SUG_PROD,
    SUG_FEAT,
    SUG_REMIX,
    SUG_PREFIX,
    SUG_SUFFIX,
    SUG_NOISE,
    SUG_LICENSE,
    NB_SUG_RULES
} suggest_rule_t;

/*
** removed : the text taken out of the title (identifies the suggestion)
** title : the title once it is applied; artist / role : artist it adds
** (empty if none)
*/
typedef struct suggest_s {
    suggest_rule_t rule;
    char removed[NAME_LEN];
    char title[NAME_LEN];
    char artist[NAME_LEN];
    int role;
} suggest_t;

typedef struct suggest_list_s {
    suggest_t items[SUGGEST_MAX];
    int nb;
} suggest_list_t;

/*
** what to look at to make the suggestions of a song
*/
typedef struct suggest_ctx_s {
    struct library_s *lib;
    song_t *song;
    char const *title;
    suggest_list_t *out;
} suggest_ctx_t;

/*
** a suggestion hidden (SUG_HIDDEN, can be shown again) or deleted
** (SUG_DELETED, never proposed again) for a song (raw id)
*/
typedef struct sug_state_s {
    int song;
    int rule;
    char *key;
    int state;
    struct sug_state_s *next;
} sug_state_t;

/*
** tiny picture (COVER_SIG x COVER_SIG, rgb) of an image, to see if two
** images are the same: cover = the cover of the mp3, image = the image
** of the song in data/images (has_* = 0 if there is none)
*/
typedef struct cover_sig_s {
    unsigned char cover[COVER_SIG * COVER_SIG * 3];
    unsigned char image[COVER_SIG * COVER_SIG * 3];
    int has_cover;
    int has_image;
} cover_sig_t;

/*
** present : the present songs sorted by title ("Tous les titres"),
** nb songs
*/
/*
** settings of the application (settings.tsv), changed on the settings
** screen; help : 1 to explain the buttons when the mouse is over them
*/
typedef struct settings_s {
    int help;
} settings_t;

typedef struct library_s {
    song_t *songs;
    fprint_t *fprints;
    artist_t *artists;
    link_t *links;
    elo_t *elo;
    playlist_t *playlists;
    board_t *boards;
    view_sort_t *sorts;
    rejected_t *rejected;
    rejected_t *excluded;
    source_t *sources;
    envelope_t *envelopes;
    not_dup_t *not_dups;
    tie_order_t *ties;
    like_t *likes;
    sug_state_t *sug_states;
    settings_t settings;
    int envelopes_changed;
    song_t **present;
    int nb;
    int next_song;
    int next_artist;
    int next_playlist;
    int next_board;
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

/*
** an ID3v2 tag read from a file: the frames start at data + start
*/
typedef struct id3_tag_s {
    unsigned char *data;
    long size;
    long start;
    int version;
} id3_tag_t;

typedef void (*tsv_fn_t)(library_t *lib, char **fields, int nb);

#endif
