/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** the choices of the author on the stats (lib->cats): counted in TOTAL,
** bar shown on the pages, quality or flaw, description; the defaults
** come from elo_categories.c, a bar is first shown if counted in TOTAL
*/

#include "mp3player.h"

/*
** the places left to the categories of the author stay empty
*/
void category_opts_default(library_t *lib)
{
    category_def_t const *def = NULL;

    memset(lib->cats, 0, sizeof(lib->cats));
    lib->nb_cats = NB_BUILTIN;
    lib->cats_read = 0;
    for (int i = 0; i < MAX_CATEGORIES; i++)
        lib->cat_order[i] = i;
    for (int i = 0; i < NB_STATS; i++) {
        def = category_def(i);
        if (!def)
            continue;
        snprintf(lib->cats[i].name, CAT_NAME_LEN, "%s", def->name);
        lib->cats[i].total = i < NB_BUILTIN && def->total;
        lib->cats[i].shown = def->total;
        lib->cats[i].flaw = def->flaw;
        snprintf(lib->cats[i].desc, DESC_LEN, "%s", def->desc);
    }
}

/*
** a tab would cut the line of the file: it becomes a space
*/
void category_set_desc(library_t *lib, int stat, char const *text)
{
    char *desc = lib->cats[stat].desc;

    snprintf(desc, DESC_LEN, "%s", text);
    for (int i = 0; desc[i]; i++)
        if (desc[i] == '\t' || desc[i] == '\n')
            desc[i] = ' ';
}
