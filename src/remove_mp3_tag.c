/*
** EPITECH PROJECT, 2025
** remove mp3 tag
** File description:
** removes the .mp3 at the endo of stored musics used for display
*/

#include "../include/mp3player.h"

char *remove_mp3_tag(char *sample)
{
    int len = strlen(sample);
    int i = 0;
    char *str = malloc(sizeof(char) * (len - 3));

    for (; i < len - 4; i++)
        str[i] = sample[i];
    str[i] = '\0';
    return str;
}
