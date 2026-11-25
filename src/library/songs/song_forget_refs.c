/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a song being forgotten (song_forget.c) is taken out of everything
** that names it: playlists and their custom orders, orders of the ties,
** artist links, notes, stays in the playlists, like, "not a duplicate" pairs, suggestion states and
** the rules of the rankings
*/

#include "mp3player.h"

/*
** returns the new number of ids, once id is taken out of ids
*/
static int drop_id(int *ids, int nb, int id)
{
    int kept = 0;

    for (int i = 0; i < nb; i++) {
        if (ids[i] == id)
            continue;
        ids[kept] = ids[i];
        kept++;
    }
    return kept;
}

static void forget_orders(library_t *lib, int id)
{
    for (playlist_t *pl = lib->playlists; pl; pl = pl->next)
        pl->nb = drop_id(pl->songs, pl->nb, id);
    for (view_sort_t *sort = lib->sorts; sort; sort = sort->next)
        sort->nb_custom = drop_id(sort->custom, sort->nb_custom, id);
    for (tie_order_t *tie = lib->ties; tie; tie = tie->next)
        if (tie->kind == KIND_SONG)
            tie->nb = drop_id(tie->ids, tie->nb, id);
}

static void forget_links(library_t *lib, int id)
{
    link_t **cur = &lib->links;
    link_t *old = NULL;

    while (*cur) {
        if ((*cur)->song_id != id) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old);
    }
}

static void forget_pairs(library_t *lib, int id)
{
    not_dup_t **cur = &lib->not_dups;
    not_dup_t *old = NULL;

    while (*cur) {
        if ((*cur)->a != id && (*cur)->b != id) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old);
    }
}

static void forget_suggestions(library_t *lib, int id)
{
    sug_state_t **cur = &lib->sug_states;
    sug_state_t *old = NULL;

    while (*cur) {
        if ((*cur)->song != id) {
            cur = &(*cur)->next;
            continue;
        }
        old = *cur;
        *cur = old->next;
        free(old->key);
        free(old);
    }
}

/*
** the rules of a ranking naming the song are removed
*/
static void forget_rules(board_t *board, int id)
{
    rule_t *next = NULL;

    for (rule_t *rule = board->rules; rule; rule = next) {
        next = rule->next;
        if (rule->type == RULE_SONG && rule->target == id)
            rule_remove(board, rule);
    }
}

void song_forget_refs(library_t *lib, int id)
{
    forget_orders(lib, id);
    forget_links(lib, id);
    forget_pairs(lib, id);
    forget_suggestions(lib, id);
    notes_remove(lib, id);
    periods_forget(lib, 0, id);
    for (board_t *board = lib->boards; board; board = board->next)
        forget_rules(board, id);
    like_put(lib, KIND_SONG, id, 0);
    likes_sync(lib);
}
