/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes categories.tsv (name, counted in TOTAL, bar shown,
** quality / flaw, description, builtin / custom: created by the author,
** column absent from the old files) and manual_stats.tsv (raw id of the
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

/*
** a category of the author is created in the order of the file
*/
void db_parse_category(library_t *lib, char **fields, int nb)
{
    int stat = 0;

    if (nb != 5 && nb != 6)
        return;
    stat = stat_find(lib, fields[0]);
    if (stat < 0 && nb == 6 && strcmp(fields[5], "custom") == 0)
        stat = category_add(lib, fields[0], "", 0);
    if (stat < 0) {
        category_merged(lib, fields, nb);
        return;
    }
    lib->cats[stat].total = stat < lib->nb_cats && atoi(fields[1]) == 1;
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
        if (!stat_exists(lib, i))
            continue;
        opt = &lib->cats[i];
        fprintf(file, "%s\t%d\t%d\t%s\t%s\t%s\n", opt->name, opt->total,
            opt->shown, opt->flaw ? "flaw" : "quality", opt->desc,
            opt->custom ? "custom" : "builtin");
    }
    return tsv_commit(file, CATEGORIES_DB);
}

void db_parse_manual(library_t *lib, char **fields, int nb)
{
    int stat = 0;

    if (nb != 3 || atoi(fields[0]) <= 0)
        return;
    stat = stat_find(lib, fields[1]);
    if (stat < MAX_CATEGORIES)
        return;
    manual_set(lib, atoi(fields[0]), stat, atoi(fields[2]));
}

int db_save_manual(library_t *lib)
{
    FILE *file = tsv_create(MANUAL_DB, MANUAL_COLUMNS);

    if (!file)
        return 84;
    for (manual_t *m = lib->manual; m; m = m->next)
        fprintf(file, "%d\t%s\t%d\n", m->song_id, stat_name(lib, m->stat),
            m->value);
    return tsv_commit(file, MANUAL_DB);
}

/*
** quality or flaw of a category in one ranking (board_flaws.c); read
** after the categories
*/
void db_parse_board_flaw(library_t *lib, char **fields, int nb)
{
    int stat = 0;

    if (nb != 3 || atoi(fields[0]) == 0)
        return;
    stat = stat_find(lib, fields[1]);
    if (stat < 0 || stat >= lib->nb_cats)
        return;
    board_flaw_set(lib, atoi(fields[0]), stat,
        strcmp(fields[2], "flaw") == 0);
}

/*
** the rankings removed since are left out
*/
int db_save_board_flaws(library_t *lib)
{
    FILE *file = tsv_create(BOARD_FLAWS_DB, BOARD_FLAWS_COLUMNS);

    if (!file)
        return 84;
    for (board_flaw_t *f = lib->board_flaws; f; f = f->next)
        if (board_by_id(lib, f->board))
            fprintf(file, "%d\t%s\t%s\n", f->board,
                category_name(lib, f->stat), f->flaw ? "flaw" : "quality");
    return tsv_commit(file, BOARD_FLAWS_DB);
}
