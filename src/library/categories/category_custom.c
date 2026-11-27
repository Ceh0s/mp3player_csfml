/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** categories created by the author: added after the others (counted in
** TOTAL, bar shown), renamed (battles.log follows, db_category_rename)
** or removed (the following ones move back; its battles stay in
** battles.log, ignored like those of a category taken out of the table)
*/

#include "mp3player.h"

/*
** 1 if name can be given to the stat except (-1 for a new one): not
** empty, not too long, no tab, not "TOTAL", not taken by another stat
*/
int category_name_free(library_t const *lib, char const *name, int except)
{
    int found = stat_find(lib, name);

    if (name[0] == '\0' || strlen(name) >= CAT_NAME_LEN
        || strchr(name, '\t') || strcmp(name, "TOTAL") == 0)
        return 0;
    return found < 0 || found == except;
}

/*
** returns its index, -1 if the name is refused or there is no room
*/
int category_add(library_t *lib, char const *name, char const *desc,
    int flaw)
{
    cat_opt_t *opt = NULL;

    if (lib->nb_cats >= MAX_CATEGORIES || !category_name_free(lib, name, -1))
        return -1;
    opt = &lib->cats[lib->nb_cats];
    memset(opt, 0, sizeof(cat_opt_t));
    snprintf(opt->name, CAT_NAME_LEN, "%s", name);
    opt->custom = 1;
    opt->total = 1;
    opt->shown = 1;
    opt->flaw = flaw;
    category_set_desc(lib, lib->nb_cats, desc);
    lib->nb_cats++;
    return lib->nb_cats - 1;
}

/*
** returns 0 if renamed (the battles follow it), 84 if refused
*/
int category_rename(library_t *lib, int cat, char const *name)
{
    char old[CAT_NAME_LEN];

    if (cat < 0 || cat >= lib->nb_cats || !lib->cats[cat].custom)
        return 84;
    if (strcmp(lib->cats[cat].name, name) == 0)
        return 0;
    if (!category_name_free(lib, name, cat))
        return 84;
    snprintf(old, sizeof(old), "%s", lib->cats[cat].name);
    snprintf(lib->cats[cat].name, CAT_NAME_LEN, "%s", name);
    db_category_rename(old, name);
    return 0;
}

/*
** the Elo are replayed: the indices of the following categories changed
*/
int category_remove(library_t *lib, int cat)
{
    if (cat < 0 || cat >= lib->nb_cats || !lib->cats[cat].custom)
        return 84;
    for (int i = cat; i + 1 < lib->nb_cats; i++)
        lib->cats[i] = lib->cats[i + 1];
    lib->nb_cats--;
    board_flaws_remove_cat(lib, cat);
    memset(&lib->cats[lib->nb_cats], 0, sizeof(cat_opt_t));
    db_save_categories(lib);
    db_save_board_flaws(lib);
    db_load_elo(lib);
    return 0;
}

/*
** where a category chosen on a screen goes after the removal of cat
*/
int category_after_remove(int category, int cat)
{
    if (category == cat)
        return CAT_ALL;
    if (category > cat && category < MAX_CATEGORIES)
        return category - 1;
    return category;
}
