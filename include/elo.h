/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** Elo ratings of the songs, one per song and per category
*/

#ifndef ELO_H_
    #define ELO_H_

    #define NB_CATEGORIES 5
    #define CAT_GLOBAL 0
    #define CAT_TOTAL NB_CATEGORIES
    #define ELO_START 1500.0
    #define ELO_K_NEW 40.0
    #define ELO_K_OLD 20.0
    #define ELO_NEW_BATTLES 10
    #define NB_TIERS 5

/*
** The ratings are never loaded from a file: they are rebuilt at every
** load by replaying data/battles.log with the resolved song ids, so a
** merge groups the ratings and a split restores them exactly.
** song_id  : resolved id of the song
** category : index in elo_categories (GLOBAL is 0)
*/
typedef struct elo_s {
    int song_id;
    int category;
    double rating;
    int battles;
    int wins;
    struct elo_s *next;
} elo_t;

/*
** score of a song in a category (TOTAL: sum of the real categories),
** used by the rankings and to choose the battle pairs
*/
typedef struct rank_s {
    struct song_s *song;
    double rating;
    int battles;
    int wins;
} rank_t;

#endif
