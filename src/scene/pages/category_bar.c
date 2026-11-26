/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** choice of the Elo category, shared by the battle and ranking screens
*/

#include "mp3player.h"

/*
** a rated category different from the current one (never "Tout
** confondu" nor the categories that do not measure the quality)
*/
static int random_category(int current)
{
    int choices[NB_CATEGORIES];
    int nb = 0;

    for (int i = 0; i < NB_CATEGORIES; i++)
        if (category_rated(i) && i != current) {
            choices[nb] = i;
            nb++;
        }
    if (nb == 0)
        return current;
    return choices[rand() % nb];
}

/*
** "<  name  >" browses the nb first categories (TOTAL is included when
** nb = NB_CATEGORIES + 1), random adds the "Catégorie aléatoire" button;
** returns 1 if the category changed
*/
int category_bar(app_t *app, int *category, int nb, int random)
{
    int old = *category;
    float dice[4] = {650, CAT_BAR_Y, 360, 56};

    if (widget_button(app, (float [4]){130, CAT_BAR_Y, 60, 56}, "<"))
        *category = (*category + nb - 1) % nb;
    widget_text(app, category_name(*category),
        (float [3]){215, CAT_BAR_Y + 6, 32}, &sfWhite);
    if (widget_button(app, (float [4]){560, CAT_BAR_Y, 60, 56}, ">"))
        *category = (*category + 1) % nb;
    if (random && widget_icon_button(app, dice, ICON_DICE,
            "Catégorie aléatoire"))
        *category = random_category(*category);
    return *category != old;
}
