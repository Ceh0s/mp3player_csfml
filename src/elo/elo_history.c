/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** history of the battles of an arena (kind and ranking, every category),
** shown by the rankings screen: battles.log is replayed again, but only
** its lines about this arena (its ratings only depend on them), with
** the same rules as db_load_elo. Each battle keeps the Elo and the place
** of both sides before and after it, and their record against each
** other. The places are counted in the pool of today (the one of that
** time is not known).
*/

#include "mp3player.h"
#include <math.h>

/*
** ratings and battles : by category, then by resolved id
** pool : ids of the pool of the arena, to count the places
*/
typedef struct replay_s {
    library_t *lib;
    arena_t arena;
    int max_id;
    double *ratings[NB_CATEGORIES];
    int *battles[NB_CATEGORIES];
    int *pool;
    int nb_pool;
    history_t *out;
} replay_t;

static void ratings_reset(replay_t *replay)
{
    for (int cat = 0; cat < NB_CATEGORIES; cat++)
        for (int id = 0; id <= replay->max_id; id++) {
            replay->ratings[cat][id] = ELO_START;
            replay->battles[cat][id] = 0;
        }
}

static hist_t *history_add(history_t *history)
{
    hist_t *items = NULL;

    if (history->nb == history->size) {
        history->size = history->size ? history->size * 2 : 256;
        items = realloc(history->items, sizeof(hist_t) * history->size);
        if (!items)
            return NULL;
        history->items = items;
    }
    memset(&history->items[history->nb], 0, sizeof(hist_t));
    history->nb++;
    return &history->items[history->nb - 1];
}

/*
** shared rank, as the leaderboard: 1 + the members with a higher
** rounded Elo
*/
static int place(replay_t const *replay, int cat, int id)
{
    long mine = lround(replay->ratings[cat][id]);
    int above = 0;

    for (int i = 0; i < replay->nb_pool; i++)
        above += lround(replay->ratings[cat][replay->pool[i]]) > mine;
    return above + 1;
}

/*
** wins of each side against the other in the category, from the last
** reset to this battle (the last item)
*/
static void face_to_face(history_t *history, hist_t *last)
{
    hist_t const *item = NULL;

    for (int i = history->nb - 1; i >= 0; i--) {
        item = &history->items[i];
        if (item->reset)
            break;
        if (item->category != last->category)
            continue;
        if (item->ids[0] == last->ids[0] && item->ids[1] == last->ids[1])
            last->wins[0]++;
        if (item->ids[0] == last->ids[1] && item->ids[1] == last->ids[0])
            last->wins[1]++;
    }
}

static void battle(replay_t *replay, long date, int cat, int const *ids)
{
    hist_t *item = history_add(replay->out);
    double ratings[2];
    int battles[2];

    if (!item)
        return;
    item->date = date;
    item->category = cat;
    for (int i = 0; i < 2; i++) {
        item->ids[i] = ids[i];
        item->before[i] = replay->ratings[cat][ids[i]];
        item->place_before[i] = place(replay, cat, ids[i]);
        ratings[i] = item->before[i];
        battles[i] = replay->battles[cat][ids[i]];
    }
    elo_update(ratings, battles);
    for (int i = 0; i < 2; i++) {
        replay->ratings[cat][ids[i]] = ratings[i];
        replay->battles[cat][ids[i]]++;
        item->after[i] = ratings[i];
    }
    for (int i = 0; i < 2; i++)
        item->place_after[i] = place(replay, cat, ids[i]);
    face_to_face(replay->out, item);
}

/*
** a reset line: timestamp, "reset", kind or "all", ranking or "all";
** returns 1 if the line was a reset
*/
static int reset_line(replay_t *replay, char **fields, int nb)
{
    int kind = -1;
    hist_t *item = NULL;

    if (nb < 4 || strcmp(fields[1], "reset") != 0)
        return 0;
    if (strcmp(fields[2], "all") != 0) {
        kind = kind_find(fields[2]);
        if (kind != (int)replay->arena.kind)
            return 1;
    }
    if (strcmp(fields[3], "all") != 0
        && atoi(fields[3]) != replay->arena.board)
        return 1;
    ratings_reset(replay);
    item = history_add(replay->out);
    if (item) {
        item->reset = 1;
        item->date = atol(fields[0]);
    }
    return 1;
}

/*
** the same rules as parse_battle (db_elo.c)
*/
static void read_line(replay_t *replay, char **fields, int nb)
{
    int cat = 0;
    int ids[2];

    if (reset_line(replay, fields, nb) || nb < 6)
        return;
    if (kind_find(fields[1]) != (int)replay->arena.kind
        || atoi(fields[2]) != replay->arena.board)
        return;
    cat = category_find(fields[3]);
    if (cat < 0 || cat >= NB_CATEGORIES)
        return;
    ids[0] = subject_resolve(replay->lib, replay->arena.kind,
        atoi(fields[4]));
    ids[1] = subject_resolve(replay->lib, replay->arena.kind,
        atoi(fields[5]));
    if (ids[0] <= 0 || ids[1] <= 0 || ids[0] == ids[1]
        || ids[0] > replay->max_id || ids[1] > replay->max_id)
        return;
    battle(replay, atol(fields[0]), cat, ids);
}

static void read_log(replay_t *replay)
{
    FILE *file = fopen(BATTLES_LOG, "r");
    char *line = NULL;
    size_t size = 0;
    char *fields[TSV_MAX_FIELDS];
    int nb = 0;

    if (!file)
        return;
    while (getline(&line, &size, file) != -1) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\0')
            continue;
        nb = split_tab(line, fields, TSV_MAX_FIELDS);
        read_line(replay, fields, nb);
    }
    free(line);
    fclose(file);
}

static int replay_create(replay_t *replay)
{
    pool_t pool = {NULL, 0};

    replay->max_id = replay->arena.kind == KIND_SONG
        ? replay->lib->next_song : replay->lib->next_artist;
    for (int cat = 0; cat < NB_CATEGORIES; cat++) {
        replay->ratings[cat] = malloc(sizeof(double) * (replay->max_id + 1));
        replay->battles[cat] = malloc(sizeof(int) * (replay->max_id + 1));
        if (!replay->ratings[cat] || !replay->battles[cat])
            return 84;
    }
    if (pool_build(replay->lib, &replay->arena, &pool) == 84)
        return 84;
    replay->pool = malloc(sizeof(int) * (pool.nb + 1));
    for (int i = 0; replay->pool && i < pool.nb; i++)
        if (rank_id(&pool.items[i]) > 0
            && rank_id(&pool.items[i]) <= replay->max_id)
            replay->pool[replay->nb_pool++] = rank_id(&pool.items[i]);
    free(pool.items);
    if (!replay->pool)
        return 84;
    ratings_reset(replay);
    return 0;
}

static void replay_destroy(replay_t *replay)
{
    for (int cat = 0; cat < NB_CATEGORIES; cat++) {
        free(replay->ratings[cat]);
        free(replay->battles[cat]);
    }
    free(replay->pool);
}

/*
** fills out (its items are to free with history_free), oldest first
*/
void elo_history(library_t *lib, arena_t const *arena, history_t *out)
{
    replay_t replay;

    memset(&replay, 0, sizeof(replay));
    memset(out, 0, sizeof(history_t));
    replay.lib = lib;
    replay.arena = *arena;
    replay.out = out;
    if (replay_create(&replay) == 0)
        read_log(&replay);
    replay_destroy(&replay);
}

void history_free(history_t *history)
{
    free(history->items);
    memset(history, 0, sizeof(history_t));
}
