/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** order in which the categories are shown (list of the categories, stat
** bars), chosen by the author by dragging them in the list: it only
** changes lib->cat_order, never the index of a category (the Elo, "Tout
** confondu" = CAT_ALL and the defaults keep theirs). Kept as the order of
** the lines of categories.tsv.
*/

#include "mp3player.h"

/*
** stat shown at place i, for i from 0 to NB_STATS: the categories in
** their order, -1 for the places left to them, then the stats set by hand
*/
int stat_at(library_t const *lib, int i)
{
    if (i < lib->nb_cats)
        return lib->cat_order[i];
    return i < MAX_CATEGORIES ? -1 : i;
}

static int category_pos(library_t const *lib, int stat)
{
    for (int i = 0; i < lib->nb_cats; i++)
        if (lib->cat_order[i] == stat)
            return i;
    return -1;
}

/*
** the category goes to place pos, those in between move by one
*/
void category_place(library_t *lib, int stat, int pos)
{
    int from = category_pos(lib, stat);

    if (from < 0 || pos < 0 || pos >= lib->nb_cats)
        return;
    while (from < pos) {
        lib->cat_order[from] = lib->cat_order[from + 1];
        from++;
    }
    while (from > pos) {
        lib->cat_order[from] = lib->cat_order[from - 1];
        from--;
    }
    lib->cat_order[pos] = stat;
}

/*
** a line of categories.tsv read: its category comes after those of the
** lines before (once: two old categories can name the same one)
*/
void category_read_order(library_t *lib, int stat)
{
    int pos = category_pos(lib, stat);

    if (pos < lib->cats_read)
        return;
    category_place(lib, stat, lib->cats_read);
    lib->cats_read++;
}

/*
** the category cat is removed (lib->nb_cats already one less): the ones
** after it go back by one index
*/
void category_order_remove(library_t *lib, int cat)
{
    int pos = 0;

    while (pos <= lib->nb_cats && lib->cat_order[pos] != cat)
        pos++;
    if (pos > lib->nb_cats)
        return;
    for (int i = pos; i + 1 < MAX_CATEGORIES; i++)
        lib->cat_order[i] = lib->cat_order[i + 1];
    for (int i = 0; i < MAX_CATEGORIES; i++)
        if (lib->cat_order[i] > cat)
            lib->cat_order[i]--;
}
