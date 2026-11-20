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

/*
** queue : the songs played one after the other, those of the playlist
** playlist_id (0 = every present song), rebuilt when the library changes
** index : position of the current song in the queue (-1 if none)
*/
typedef struct player_s {
    sfMusic *music;
    song_t *song;
    song_t **queue;
    int nb;
    int playlist_id;
    int index;
    long session_ms;
    long carry_us;
} player_t;

#endif
