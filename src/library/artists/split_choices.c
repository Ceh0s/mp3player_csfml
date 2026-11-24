/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** choices kept for the artist names written as a list (split or keep
** whole, artist_splits.tsv), see artist_split.c
*/

#include "mp3player.h"
#include <strings.h>

split_choice_t *split_choice_find(library_t *lib, char const *name)
{
    for (split_choice_t *c = lib->splits; c; c = c->next)
        if (strcasecmp(c->name, name) == 0)
            return c;
    return NULL;
}

void split_choice_set(library_t *lib, char const *name, int keep)
{
    split_choice_t *choice = split_choice_find(lib, name);

    if (choice) {
        choice->keep = keep;
        return;
    }
    choice = calloc(1, sizeof(split_choice_t));
    if (!choice)
        return;
    choice->name = strdup(name);
    if (!choice->name) {
        free(choice);
        return;
    }
    choice->keep = keep;
    choice->next = lib->splits;
    lib->splits = choice;
}

void split_choices_free(library_t *lib)
{
    split_choice_t *next = NULL;

    while (lib->splits) {
        next = lib->splits->next;
        free(lib->splits->name);
        free(lib->splits);
        lib->splits = next;
    }
}

/*
** artist_splits.tsv: name, "keep" or "split"
*/
void db_parse_split(library_t *lib, char **fields, int nb)
{
    if (nb != 2 || fields[0][0] == '\0')
        return;
    split_choice_set(lib, fields[0], strcmp(fields[1], "keep") == 0);
}

int db_save_splits(library_t *lib)
{
    FILE *file = tsv_create(SPLITS_DB, SPLITS_COLUMNS);

    if (!file)
        return 84;
    for (split_choice_t *c = lib->splits; c; c = c->next)
        fprintf(file, "%s\t%s\n", c->name, c->keep ? "keep" : "split");
    return tsv_commit(file, SPLITS_DB);
}
