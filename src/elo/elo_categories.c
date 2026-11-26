/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** categories of the battles: "Tout confondu" (free judgement, independent)
** + the others, then the stats set by hand (not Elo, see manual_stats.c).
** Default values here, changed by the author in data/categories.tsv
** (category_opts.c). To add one, append it to the table and increase
** NB_CATEGORIES (or NB_MANUAL) in elo.h; files store the names, so the
** order can change without breaking them. Also the names of the kinds.
*/

#include "mp3player.h"

static const category_def_t defs[NB_STATS] = {
    {"Tout confondu", 0, 0, "Jugement libre, sans critère précis."},
    {"Mélodie", 1, 0, "Thèmes, lignes mélodiques, harmonie."},
    {"Production", 1, 0, "Son, mixage, arrangement."},
    {"Émotion", 1, 0, "Ce qu'elle fait ressentir."},
    {"Rejouabilité", 1, 0, "Supporte d'être enchaînée sans lasser."},
    {"Mémorabilité", 1, 0, "Reste en tête, on s'en souvient longtemps."},
    {"Fidélité", 1, 0,
        "Revient dans les écoutes au fil des mois et des années."},
    {"Nostalgie", 1, 0, "Rappelle une époque, un souvenir."},
    {"Obsession", 0, 0, "A tourné en boucle à un moment."},
    {"Meme material", 0, 0, "Potentiel comique, référence."},
    {"Agression auditive", 0, 0, "Intensité, violence du son."},
    {"Entêtement", 0, 1,
        "Colle à la tête sans qu'on le veuille, finit par agacer."},
    {"Lassitude", 0, 1, "Usée à force d'être écoutée : elle lasse maintenant."},
    {"Biais de récence", 0, 1,
        "Surévaluée au début parce qu'elle était nouvelle, sans l'avoir usée."}
};

category_def_t const *category_def(int stat)
{
    if (stat < 0 || stat >= NB_STATS)
        return NULL;
    return &defs[stat];
}

/*
** returns the index of the battle category, -1 if it does not exist
** anymore
*/
int category_find(char const *name)
{
    for (int i = 0; i < NB_CATEGORIES; i++)
        if (strcmp(defs[i].name, name) == 0)
            return i;
    return -1;
}

/*
** the same among all the stats (those set by hand too)
*/
int stat_find(char const *name)
{
    for (int i = 0; i < NB_STATS; i++)
        if (strcmp(defs[i].name, name) == 0)
            return i;
    return -1;
}

/*
** TOTAL is virtual: the sum of the categories counted in it, never stored
** (its index is the one of the first stat set by hand: see stat_name)
*/
char const *category_name(int category)
{
    if (category == CAT_TOTAL)
        return "TOTAL";
    if (category < 0 || category > CAT_TOTAL)
        return "?";
    return defs[category].name;
}

/*
** name of any stat, those set by hand included
*/
char const *stat_name(int stat)
{
    if (stat < 0 || stat >= NB_STATS)
        return "?";
    return defs[stat].name;
}

/*
** 1 if the battle category is counted in TOTAL and drawn by "Catégorie
** aléatoire" (the others are only chosen by hand)
*/
int category_rated(library_t const *lib, int category)
{
    if (category < 0 || category >= NB_CATEGORIES)
        return 0;
    return lib->cats[category].total;
}

/*
** a category counted in TOTAL different from the current one (never
** "Tout confondu" unless the author counted it)
*/
int category_random(library_t const *lib, int current)
{
    int choices[NB_CATEGORIES];
    int nb = 0;

    for (int i = 0; i < NB_CATEGORIES; i++)
        if (category_rated(lib, i) && i != current) {
            choices[nb] = i;
            nb++;
        }
    if (nb == 0)
        return current;
    return choices[rand() % nb];
}

static char const *const kind_names[NB_KINDS] = {"song", "artist"};

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
