/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** choice of the Elo category, shared by the battle and ranking screens:
** a button opening the list of the categories (category_list.c), and
** the description of the one chosen under it
*/

#include "mp3player.h"

static const sfColor gray = {190, 190, 190, 255};

/*
** a category counted in TOTAL different from the current one (never
** "Tout confondu" unless the author counted it)
*/
static int random_category(app_t *app, int current)
{
    int choices[NB_CATEGORIES];
    int nb = 0;

    for (int i = 0; i < NB_CATEGORIES; i++)
        if (category_rated(app->lib, i) && i != current) {
            choices[nb] = i;
            nb++;
        }
    if (nb == 0)
        return current;
    return choices[rand() % nb];
}

/*
** random adds the "Catégorie aléatoire" button (battle screen: the
** description goes right of it, above the cards); returns 1 if the
** category changed (the list returns it on its own, category_list)
*/
int category_bar(app_t *app, int *category, int random)
{
    int old = *category;
    float button[4] = {130, CAT_BAR_Y, 490, 56};
    float dice[4] = {650, CAT_BAR_Y, 360, 56};
    char const *desc = *category < NB_CATEGORIES
        ? app->lib->cats[*category].desc : "Somme des catégories cochées.";

    widget_tip(app, button, desc);
    if (widget_icon_button(app, button, ICON_LIST, category_name(*category)))
        app->catlist.open = !app->catlist.open;
    if (random)
        widget_fit(app, desc, (float [4]){1030, CAT_BAR_Y + 16, 20, 750},
            &gray);
    else
        widget_fit(app, desc, (float [4]){130, CAT_BAR_Y + 62, 20, 1650},
            &gray);
    if (random && widget_icon_button(app, dice, ICON_DICE,
            "Catégorie aléatoire"))
        *category = random_category(app, *category);
    return *category != old;
}
