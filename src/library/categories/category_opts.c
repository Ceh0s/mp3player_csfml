/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** the choices of the author on the stats (lib->cats): counted in TOTAL,
** bar shown on the pages, quality or flaw, description; the defaults
** come from elo_categories.c, a bar is first shown if counted in TOTAL
*/

#include "mp3player.h"

void category_opts_default(library_t *lib)
{
    category_def_t const *def = NULL;

    for (int i = 0; i < NB_STATS; i++) {
        def = category_def(i);
        lib->cats[i].total = i < NB_CATEGORIES && def->total;
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
