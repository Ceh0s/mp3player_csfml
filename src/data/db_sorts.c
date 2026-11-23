/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes playlist_sorts.tsv (criterion and direction of each
** playlist) and custom_orders.tsv (the custom orders, kept apart from
** the playlists: their own order stays the order of addition)
*/

#include "mp3player.h"

/*
** fields: playlist_id, key, direction; a deleted playlist is ignored
*/
void db_parse_sort(library_t *lib, char **fields, int nb)
{
    int id = 0;
    int key = 0;
    view_sort_t *sort = NULL;

    if (nb != 3)
        return;
    id = atoi(fields[0]);
    key = sort_key_by_name(fields[1]);
    if (key < 0 || (id != 0 && !playlist_by_id(lib, id)))
        return;
    sort = sort_get(lib, id);
    if (!sort)
        return;
    sort->key = key;
    sort->desc = strcmp(fields[2], "desc") == 0;
}

/*
** fields: playlist_id, song_id, position (the lines are in order)
*/
void db_parse_custom(library_t *lib, char **fields, int nb)
{
    int id = 0;
    view_sort_t *sort = NULL;
    int *bigger = NULL;

    if (nb < 2 || atoi(fields[1]) <= 0)
        return;
    id = atoi(fields[0]);
    sort = id == 0 || playlist_by_id(lib, id) ? sort_get(lib, id) : NULL;
    if (!sort)
        return;
    bigger = realloc(sort->custom, sizeof(int) * (sort->nb_custom + 1));
    if (!bigger)
        return;
    sort->custom = bigger;
    sort->custom[sort->nb_custom] = atoi(fields[1]);
    sort->nb_custom++;
}

static int save_custom(library_t *lib)
{
    FILE *file = tsv_create(CUSTOM_DB, CUSTOM_COLUMNS);

    if (!file)
        return 84;
    for (view_sort_t *sort = lib->sorts; sort; sort = sort->next)
        for (int i = 0; i < sort->nb_custom; i++)
            fprintf(file, "%d\t%d\t%d\n", sort->playlist_id,
                sort->custom[i], i + 1);
    return tsv_commit(file, CUSTOM_DB);
}

int db_save_sorts(library_t *lib)
{
    FILE *file = tsv_create(SORTS_DB, SORTS_COLUMNS);

    if (!file)
        return 84;
    for (view_sort_t *sort = lib->sorts; sort; sort = sort->next)
        fprintf(file, "%d\t%s\t%s\n", sort->playlist_id,
            sort_def(sort->key)->name, sort->desc ? "desc" : "asc");
    if (tsv_commit(file, SORTS_DB) == 84)
        return 84;
    return save_custom(lib);
}
