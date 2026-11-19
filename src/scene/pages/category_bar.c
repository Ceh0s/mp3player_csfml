/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** choice of the Elo category, shared by the battle and ranking screens
*/

#include "mp3player.h"

/*
** a real category different from the current one (never GLOBAL)
*/
static int random_category(int current)
{
    int category = current;

    if (NB_CATEGORIES < 3)
        return CAT_GLOBAL + 1;
    while (category == current)
        category = CAT_GLOBAL + 1 + rand() % (NB_CATEGORIES - 1);
    return category;
}

/*
** "<  name  >" browses the nb first categories (TOTAL is included when
** nb = NB_CATEGORIES + 1), random adds the "Catégorie aléatoire" button;
** returns 1 if the category changed
*/
int category_bar(app_t *app, int *category, int nb, int random)
{
    int old = *category;

    if (widget_button(app, (float [4]){130, 190, 60, 56}, "<"))
        *category = (*category + nb - 1) % nb;
    widget_text(app, category_name(*category), (float [3]){215, 196, 32},
        &sfWhite);
    if (widget_button(app, (float [4]){560, 190, 60, 56}, ">"))
        *category = (*category + 1) % nb;
    if (random && widget_button(app, (float [4]){650, 190, 360, 56},
            "Catégorie aléatoire"))
        *category = random_category(*category);
    return *category != old;
}
