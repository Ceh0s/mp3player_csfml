/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** Elo ratings of the songs and artists, one per ranking and per category
*/

#ifndef ELO_H_
    #define ELO_H_

    #define NB_CATEGORIES 6
    #define CAT_ALL 0
    #define CAT_TOTAL NB_CATEGORIES
    #define BOARD_GENERAL 0
    #define ELO_START 1500.0
    #define ELO_K_NEW 40.0
    #define ELO_K_OLD 20.0
    #define ELO_NEW_BATTLES 10
    #define NB_TIERS 5

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
** the songs or artists that can meet in the battles of an arena
*/
typedef struct pool_s {
    rank_t *items;
    int nb;
} pool_t;

#endif
