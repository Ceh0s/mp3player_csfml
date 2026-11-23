/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes suggestions.tsv: the title suggestions hidden or
** deleted (raw song id, rule, removed text, hidden / deleted)
*/

#include "mp3player.h"

void db_parse_suggestion(library_t *lib, char **fields, int nb)
{
    suggest_t sug;

    if (nb != 4 || atoi(fields[0]) <= 0)
        return;
    memset(&sug, 0, sizeof(sug));
    sug.rule = suggest_rule_find(fields[1]);
    if ((int)sug.rule < 0)
        return;
    snprintf(sug.removed, NAME_LEN, "%s", fields[2]);
    suggest_state_add(lib, atoi(fields[0]), &sug,
        strcmp(fields[3], "deleted") == 0 ? SUG_DELETED : SUG_HIDDEN);
}

int db_save_suggestions(library_t *lib)
{
    FILE *file = tsv_create(SUGGEST_DB, SUGGEST_COLUMNS);

    if (!file)
        return 84;
    for (sug_state_t *st = lib->sug_states; st; st = st->next)
        if (st->state != SUG_VISIBLE)
            fprintf(file, "%d\t%s\t%s\t%s\n", st->song,
                suggest_rule_name(st->rule), st->key,
                st->state == SUG_DELETED ? "deleted" : "hidden");
    return tsv_commit(file, SUGGEST_DB);
}
