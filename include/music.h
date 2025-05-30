/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** local window struct
*/

#ifndef MUSIC_H_
    #define MUSIC_H_

    #include <SFML/Graphics.h>

typedef struct music_s {
    char **music;
    int nb;
    sfFont *font;
    sfText **text;
} music_t;

#endif
