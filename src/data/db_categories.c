/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes categories.tsv (name, counted in TOTAL, bar shown,
** quality / flaw, description) and manual_stats.tsv (raw id of the
** song, name of the stat, value from 0 to 100)
*/

#include "mp3player.h"

/*
** descriptions given by default before: still the same in the file, they
** were not written by the author and take the new default
*/
static char const *const old_defaults[][2] = {
    {"Lassitude", "Moins aimée qu'au début : elle a fini par lasser."},
    {"Biais de récence",
        "Part de l'appréciation due à la nouveauté, retombée depuis."}
};

static int old_default(char const *name, char const *desc)
{
    int nb = sizeof(old_defaults) / sizeof(old_defaults[0]);

    for (int i = 0; i < nb; i++)
        if (strcmp(old_defaults[i][0], name) == 0
            && strcmp(old_defaults[i][1], desc) == 0)
            return 1;
    return 0;
}

void db_parse_category(library_t *lib, char **fields, int nb)
{
    int stat = 0;

    if (nb != 5)
        return;
    stat = stat_find(fields[0]);
    if (stat < 0)
        return;
    lib->cats[stat].total = stat < NB_CATEGORIES && atoi(fields[1]) == 1;
    lib->cats[stat].shown = atoi(fields[2]) == 1;
    lib->cats[stat].flaw = strcmp(fields[3], "flaw") == 0;
    if (!old_default(fields[0], fields[4]))
        category_set_desc(lib, stat, fields[4]);
}

int db_save_categories(library_t *lib)
{
    FILE *file = tsv_create(CATEGORIES_DB, CATEGORIES_COLUMNS);
    cat_opt_t const *opt = NULL;

    if (!file)
        return 84;
    for (int i = 0; i < NB_STATS; i++) {
        opt = &lib->cats[i];
        fprintf(file, "%s\t%d\t%d\t%s\t%s\n", stat_name(i), opt->total,
            opt->shown, opt->flaw ? "flaw" : "quality", opt->desc);
    }
    return tsv_commit(file, CATEGORIES_DB);
}

void db_parse_manual(library_t *lib, char **fields, int nb)
{
    int stat = 0;

    if (nb != 3 || atoi(fields[0]) <= 0)
        return;
    stat = stat_find(fields[1]);
    if (stat < NB_CATEGORIES)
        return;
    manual_set(lib, atoi(fields[0]), stat, atoi(fields[2]));
}

int db_save_manual(library_t *lib)
{
    FILE *file = tsv_create(MANUAL_DB, MANUAL_COLUMNS);

    if (!file)
        return 84;
    for (manual_t *m = lib->manual; m; m = m->next)
        fprintf(file, "%d\t%s\t%d\n", m->song_id, stat_name(m->stat),
            m->value);
    return tsv_commit(file, MANUAL_DB);
}
