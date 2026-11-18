/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** audio player struct
*/

#ifndef PLAYER_H_
    #define PLAYER_H_

    #include <SFML/Audio.h>
    #include "song.h"

typedef enum end_reason_e {
    END_COMPLETE,
    END_SKIP,
    END_QUIT
} end_reason_t;

typedef struct player_s {
    sfMusic *music;
    song_t *song;
    int index;
    long session_ms;
    long carry_us;
} player_t;

#endif
