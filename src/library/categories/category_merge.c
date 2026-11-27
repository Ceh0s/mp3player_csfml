/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** categories taken out of the table and merged into a category of the
** author (2026-11-27: "Mélodie" + "Production" -> "PLACEHOLDER", to be
** renamed). Done once: while categories.tsv still names an old category,
** the new one is created with its choices (descriptions put together),
** then the battles of the old ones are renamed in the logs, before the
** Elo are replayed. categories.tsv written again no longer names them.
*/

#include "mp3player.h"

static char const *const merged[][2] = {
    {"Mélodie", "PLACEHOLDER"},
    {"Production", "PLACEHOLDER"}
};

    #define NB_MERGED (int)(sizeof(merged) / sizeof(merged[0]))

static int merged_index(char const *name)
{
    for (int i = 0; i < NB_MERGED; i++)
        if (strcmp(merged[i][0], name) == 0)
            return i;
    return -1;
}

/*
** the second description goes after the first one
*/
static void add_desc(library_t *lib, int stat, char const *desc)
{
    char text[DESC_LEN];
    char const *old = lib->cats[stat].desc;

    if (!desc[0])
        return;
    if (old[0])
        snprintf(text, sizeof(text), "%s ; %s", old, desc);
    else
        snprintf(text, sizeof(text), "%s", desc);
    category_set_desc(lib, stat, text);
}

/*
** a line of categories.tsv naming an old category (fields: name, total,
** shown, measure, description[, origin]); returns the index of the
** category it is merged into, -1 if it is not an old one
*/
int category_merged(library_t *lib, char **fields, int nb)
{
    int i = merged_index(fields[0]);
    int stat = 0;
    int created = 0;

    if (i < 0 || (nb == 6 && strcmp(fields[5], "custom") == 0))
        return -1;
    stat = stat_find(lib, merged[i][1]);
    if (stat < 0) {
        stat = category_add(lib, merged[i][1], "", 0);
        created = 1;
    }
    if (stat < 0 || stat >= lib->nb_cats)
        return -1;
    if (created) {
        lib->cats[stat].total = atoi(fields[1]) == 1;
        lib->cats[stat].shown = atoi(fields[2]) == 1;
        lib->cats[stat].flaw = strcmp(fields[3], "flaw") == 0;
    }
    add_desc(lib, stat, fields[4]);
    lib->cats_merged = 1;
    return stat;
}

/*
** the battles follow, then categories.tsv is written without the old ones
*/
void category_merge_logs(library_t *lib)
{
    if (!lib->cats_merged)
        return;
    for (int i = 0; i < NB_MERGED; i++)
        db_category_rename(merged[i][0], merged[i][1]);
    db_save_categories(lib);
    lib->cats_merged = 0;
}
