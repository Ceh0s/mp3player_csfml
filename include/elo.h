/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** Elo ratings of the songs and artists, one per ranking and per category
*/

#ifndef ELO_H_
    #define ELO_H_

    #define NB_CATEGORIES 13
    #define NB_MANUAL 1
    #define NB_STATS (NB_CATEGORIES + NB_MANUAL)
    #define STAT_RECENCY NB_CATEGORIES
    #define DESC_LEN 160
    #define CAT_ALL 0
    #define CAT_TOTAL NB_CATEGORIES
    #define BOARD_GENERAL 0
    #define ELO_START 1500.0
    #define ELO_K_NEW 40.0
    #define ELO_K_OLD 20.0
    #define ELO_NEW_BATTLES 10
    #define NB_TIERS 6
    #define UNDO_MAX 3
    #define TIER_UNRANKED NB_TIERS

/*
** a stat of the songs: the battle categories (NB_CATEGORIES), then those
** set by hand (NB_MANUAL, not Elo); default values (elo_categories.c),
** changed by the author in data/categories.tsv: total : counted in TOTAL
** (and drawn by "Catégorie aléatoire"), flaw : 1 if it measures a flaw
** (its bar is red when high, it lowers TOTAL)
*/
typedef struct category_def_s {
    char const *name;
    int total;
    int flaw;
    char const *desc;
} category_def_t;

/*
** the same, as the author set them; shown : its bar is drawn on the pages
** of the songs and artists
*/
typedef struct cat_opt_s {
    int total;
    int shown;
    int flaw;
    char desc[DESC_LEN];
} cat_opt_t;

/*
** a stat set by hand: raw id of the song, index of the stat (>=
** NB_CATEGORIES), value from 0 to 100
*/
typedef struct manual_s {
    int song_id;
    int stat;
    int value;
    struct manual_s *next;
} manual_t;

typedef enum elo_kind_e {
    KIND_SONG,
    KIND_ARTIST,
    NB_KINDS
} elo_kind_t;

/*
** where a battle takes place: what is compared (songs or artists), in
** which ranking (board, 0 = the general one) and on which category
*/
typedef struct arena_s {
    elo_kind_t kind;
    int board;
    int category;
} arena_t;

/*
** The ratings are never loaded from a file: they are rebuilt at every
** load by replaying data/battles.log with the resolved ids, so a merge
** groups the ratings and a split restores them exactly.
** subject  : resolved id of the song, or id of the artist
** category : index in elo_categories (CAT_ALL "Tout confondu" is 0)
*/
typedef struct elo_s {
    elo_kind_t kind;
    int board;
    int subject;
    int category;
    double rating;
    int battles;
    int wins;
    struct elo_s *next;
} elo_t;

/*
** score of a song or of an artist (the other pointer is NULL) in a
** category (TOTAL: sum of the real categories), used by the rankings
** and to choose the battle pairs
*/
typedef struct rank_s {
    struct song_s *song;
    struct artist_s *artist;
    double rating;
    int battles;
    int wins;
} rank_t;

/*
** a score and its place in the order chosen between ex aequo (the
** lower first), to sort a ranking
*/
typedef struct ranked_s {
    rank_t rank;
    long tie;
} ranked_t;

/*
** the songs or artists that can meet in the battles of an arena
*/
typedef struct pool_s {
    rank_t *items;
    int nb;
} pool_t;

/*
** a battle of the history of an arena (elo_history.c), or a reset of its
** ratings (reset = 1, only date is set); [0] is the winner, [1] the
** loser: resolved ids, Elo and place (shared rank, as the leaderboard)
** before and after, and their wins against each other in this category
** since the last reset, this battle included
** line : its line in battles.log (0 = the first one), see elo_undo.c
*/
typedef struct hist_s {
    long date;
    int line;
    int reset;
    int category;
    int ids[2];
    double before[2];
    double after[2];
    int place_before[2];
    int place_after[2];
    int wins[2];
} hist_t;

typedef struct history_s {
    hist_t *items;
    int nb;
    int size;
} history_t;

#endif
