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
** fields: timestamp, category, winner, loser, winner_before, loser_before
** a battle between two songs merged since then is ignored, as well as a
** category that does not exist anymore (the line stays in the log)
*/
static void parse_battle(library_t *lib, char **fields, int nb)
{
    int category = 0;
    song_t *winner = NULL;
    song_t *loser = NULL;

    if (nb < 4)
        return;
    category = category_find(fields[1]);
    winner = song_resolve(lib, song_by_id(lib, atoi(fields[2])));
    loser = song_resolve(lib, song_by_id(lib, atoi(fields[3])));
    if (category < 0 || !winner || !loser || winner == loser)
        return;
    elo_battle(lib, winner->id, loser->id, category);
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
        fprintf(file, "%d\t%s\t%.1f\t%d\t%d\n", elo->song_id,
            elo_categories[elo->category], elo->rating, elo->battles,
            elo->wins);
    return tsv_commit(file, ELO_DB);
}

/*
** the log keeps the song ids as they are: as it is replayed with the
** resolved ids, a later merge or split also applies to this battle
*/
void battle_record(library_t *lib, song_t *winner, song_t *loser,
    int category)
{
    char line[160];
    rank_t before[2];

    elo_score(lib, winner, category, &before[0]);
    elo_score(lib, loser, category, &before[1]);
    snprintf(line, sizeof(line), "%s\t%d\t%d\t%.1f\t%.1f",
        elo_categories[category], winner->id, loser->id, before[0].rating,
        before[1].rating);
    log_line(BATTLES_LOG, BATTLES_COLUMNS, line);
    elo_battle(lib, winner->id, loser->id, category);
    db_save_elo(lib);
}
