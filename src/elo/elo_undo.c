/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** undo of the last battles of an arena (kind and ranking, every
** category), UNDO_MAX in a row at most: the line leaves battles.log, as
** it is, for battles_undone.log (nothing is lost), and the Elo are
** replayed without it. It is written there before it is removed, and
** battles.log is replaced at once (<log>.tmp, then renamed).
*/

#include "mp3player.h"

/*
** 1 if the line (cut here) is a battle of the kind and ranking
*/
static int in_arena(arena_t const *arena, char **fields, int nb)
{
    return nb >= 6 && kind_find(fields[1]) == (int)arena->kind
        && atoi(fields[2]) == arena->board;
}

/*
** how many battles may still be undone, last being the last one left:
** those undone in a row count (each one is older than the one before,
** a later one starts a new row), a new vote gives them all back
*/
int battle_undo_left(arena_t const *arena, hist_t const *last)
{
    FILE *file = fopen(BATTLES_UNDONE, "r");
    char *fields[TSV_MAX_FIELDS];
    char *line = NULL;
    size_t size = 0;
    long date = 0;
    int row = 0;
    int nb = 0;

    while (file && getline(&line, &size, file) > 0) {
        if (line[0] == '#')
            continue;
        nb = split_tab(line, fields, TSV_MAX_FIELDS);
        if (!in_arena(arena, fields, nb))
            continue;
        row = atol(fields[0]) <= date ? row + 1 : 1;
        date = atol(fields[0]);
    }
    free(line);
    if (file)
        fclose(file);
    if (date < last->date)
        row = 0;
    return row < UNDO_MAX ? UNDO_MAX - row : 0;
}

/*
** the line read again must be the battle shown
*/
static int is_battle(library_t *lib, arena_t const *arena, char const *line,
    hist_t const *item)
{
    char *copy = strdup(line);
    char *fields[TSV_MAX_FIELDS];
    int ok = 0;

    if (!copy)
        return 0;
    ok = in_arena(arena, fields, split_tab(copy, fields, TSV_MAX_FIELDS))
        && atol(fields[0]) == item->date
        && category_find(fields[3]) == item->category
        && subject_resolve(lib, arena->kind, atoi(fields[4]))
        == item->ids[0]
        && subject_resolve(lib, arena->kind, atoi(fields[5]))
        == item->ids[1];
    free(copy);
    return ok;
}

/*
** returns 84 if the line could not be written in battles_undone.log
*/
static int keep_line(char const *line)
{
    FILE *file = NULL;
    int failed = 0;

    tsv_touch(BATTLES_UNDONE, BATTLES_COLUMNS);
    file = fopen(BATTLES_UNDONE, "a");
    if (!file)
        return 84;
    failed = fputs(line, file) < 0;
    failed |= fclose(file) != 0;
    return failed ? 84 : 0;
}

/*
** battles.log is copied in out without the line of the battle, kept
** before; returns 0 if this line was found and everything was written
*/
static int copy_without(library_t *lib, arena_t const *arena,
    hist_t const *item, FILE *files[2])
{
    char *line = NULL;
    size_t size = 0;
    int found = 0;
    int failed = 0;

    for (int i = 0; getline(&line, &size, files[0]) > 0; i++) {
        if (i == item->line && is_battle(lib, arena, line, item)) {
            found = keep_line(line) == 0;
            if (found)
                continue;
        }
        failed |= fputs(line, files[1]) < 0;
    }
    failed |= ferror(files[0]) != 0;
    free(line);
    return found && !failed ? 0 : 84;
}

/*
** item : battle of the history of the arena; returns 84, with
** battles.log left as it was, if it is not found there any more
*/
int battle_undo(library_t *lib, arena_t const *arena, hist_t const *item)
{
    char const *tmp = BATTLES_LOG ".tmp";
    FILE *files[2] = {fopen(BATTLES_LOG, "r"), NULL};
    int ret = 84;

    files[1] = files[0] ? fopen(tmp, "w") : NULL;
    if (files[1]) {
        ret = copy_without(lib, arena, item, files);
        if (fclose(files[1]) != 0)
            ret = 84;
        if (ret == 0 && rename(tmp, BATTLES_LOG) != 0)
            ret = 84;
        if (ret == 84)
            remove(tmp);
    }
    if (files[0])
        fclose(files[0]);
    if (ret == 84)
        return 84;
    db_load_elo(lib);
    db_save_elo(lib);
    return 0;
}
