/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** categories of the battles: GLOBAL (independent) + the real ones
** (to add one, append its name and increase NB_CATEGORIES in elo.h);
** files store the names, so the order can change without breaking them
*/

#include "mp3player.h"

const char *const elo_categories[NB_CATEGORIES] = {
    "GLOBAL",
    "Qualité mélodique",
    "Impact émotionnel",
    "Vibe",
    "Nostalgie"
};

/*
** returns the index of the category, -1 if it does not exist anymore
*/
int category_find(char const *name)
{
    for (int i = 0; i < NB_CATEGORIES; i++)
        if (strcmp(elo_categories[i], name) == 0)
            return i;
    return -1;
}

/*
** TOTAL is virtual: the sum of the real categories, never stored
*/
char const *category_name(int category)
{
    if (category == CAT_TOTAL)
        return "TOTAL";
    if (category < 0 || category > CAT_TOTAL)
        return "?";
    return elo_categories[category];
}
