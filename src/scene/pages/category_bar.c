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
** "Catégorie aléatoire": on, each new battle draws its category
** (battle_next); turned on, it draws one at once
*/
static void random_toggle(app_t *app, int *category)
{
    float dice[4] = {650, CAT_BAR_Y, 460, 56};
    int *on = &app->lib->settings.battle_random;

    if (!widget_icon_button(app, dice, ICON_DICE, *on
            ? "Catégorie aléatoire : oui" : "Catégorie aléatoire : non"))
        return;
    *on = !*on;
    db_save_settings(app->lib);
    if (*on)
        *category = category_random(app->lib, *category);
}

/*
** random adds the "Catégorie aléatoire" toggle (battle screen: the
** description is written under the cards, battle_view.c); returns 1 if
** the category changed (the list returns it on its own, category_list)
*/
int category_bar(app_t *app, int *category, int random)
{
    int old = *category;
    float button[4] = {130, CAT_BAR_Y, 490, 56};
    char const *desc = *category < NB_CATEGORIES
        ? app->lib->cats[*category].desc : "Somme des catégories cochées.";

    widget_tip(app, button, desc);
    if (widget_icon_button(app, button, ICON_LIST, category_name(*category)))
        app->catlist.open = !app->catlist.open;
    if (random)
        random_toggle(app, category);
    else
        widget_fit(app, desc, (float [4]){130, CAT_BAR_Y + 62, 20, 1650},
            &gray);
    return *category != old;
}
