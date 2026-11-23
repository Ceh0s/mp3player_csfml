/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** categories of the battles: "Tout confondu" (free judgement, independent)
** + the real ones, chosen to measure different things so that TOTAL
** does not count the same thing twice, + those that do not measure the
** quality ("Meme material", "Agression auditive"). To add one, append
** its name and its rated value and increase NB_CATEGORIES in elo.h;
** files store the names, so the order can change without breaking
** them. Also the names of the kinds.
*/

#include "mp3player.h"

const char *const elo_categories[NB_CATEGORIES] = {
    "Tout confondu",
    "Mélodie",
    "Production",
    "Émotion",
    "Rejouabilité",
    "Nostalgie",
    "Meme material",
    "Agression auditive"
};

/*
** 1 if the category measures the quality: counted in TOTAL and drawn by
** "Catégorie aléatoire" (the others are only chosen by hand)
*/
static const int rated[NB_CATEGORIES] = {0, 1, 1, 1, 1, 1, 0, 0};

static char const *const kind_names[NB_KINDS] = {"song", "artist"};

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

int category_rated(int category)
{
    if (category < 0 || category >= NB_CATEGORIES)
        return 0;
    return rated[category];
}

/*
** returns the kind written in the files, -1 if unknown
*/
int kind_find(char const *name)
{
    for (int i = 0; i < NB_KINDS; i++)
        if (strcmp(kind_names[i], name) == 0)
            return i;
    return -1;
}

char const *kind_name(elo_kind_t kind)
{
    return kind_names[kind];
}
