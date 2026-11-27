/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** battles.log is the source of the Elo ratings: it is replayed at every
** load and after every merge or split; elo.tsv is only a readable copy
** of the result, rewritten after each battle and never read back
*/

#include "mp3player.h"

/*
** fields: timestamp, kind, board, category, winner, loser,
** winner_before, loser_before
** a battle between two songs merged since then is ignored, as well as a
** category or a ranking that does not exist anymore (the line stays in
** the log); the id of a removed ranking is never given again
*/
static void parse_battle(library_t *lib, char **fields, int nb)
{
    arena_t arena;
    int kind = 0;
    int winner = 0;
    int loser = 0;

    if (elo_parse_reset(lib, fields, nb) || nb < 6)
        return;
    kind = kind_find(fields[1]);
    arena.kind = kind;
    arena.board = atoi(fields[2]);
    board_seen_id(lib, arena.board);
    arena.category = category_find(lib, fields[3]);
    if (kind < 0 || arena.category < 0 || !board_by_id(lib, arena.board))
        return;
    winner = subject_resolve(lib, arena.kind, atoi(fields[4]));
    loser = subject_resolve(lib, arena.kind, atoi(fields[5]));
    if (winner == 0 || loser == 0 || winner == loser)
        return;
    elo_battle(lib, &arena, winner, loser);
}

void db_load_elo(library_t *lib)
{
    elo_clear(lib);
    tsv_read(BATTLES_LOG, &parse_battle, lib);
}

int db_save_elo(library_t *lib)
{
    FILE *file = tsv_create(ELO_DB, ELO_COLUMNS);

    if (!file)
        return 84;
    for (elo_t *elo = lib->elo; elo; elo = elo->next)
        fprintf(file, "%s\t%d\t%d\t%s\t%.1f\t%d\t%d\n", kind_name(elo->kind),
            elo->board, elo->subject, category_name(lib, elo->category),
            elo->rating, elo->battles, elo->wins);
    return tsv_commit(file, ELO_DB);
}

/*
** the log keeps the ids as they are: as it is replayed with the
** resolved ids, a later merge or split also applies to this battle
*/
void battle_record(library_t *lib, arena_t const *arena,
    rank_t const *winner, rank_t const *loser)
{
    char line[160];
    rank_t before[2] = {*winner, *loser};

    elo_score(lib, arena, &before[0]);
    elo_score(lib, arena, &before[1]);
    snprintf(line, sizeof(line), "%s\t%d\t%s\t%d\t%d\t%.1f\t%.1f",
        kind_name(arena->kind), arena->board,
        category_name(lib, arena->category), rank_id(winner), rank_id(loser),
        before[0].rating, before[1].rating);
    log_line(BATTLES_LOG, BATTLES_COLUMNS, line);
    elo_battle(lib, arena, rank_id(winner), rank_id(loser));
    db_save_elo(lib);
}
