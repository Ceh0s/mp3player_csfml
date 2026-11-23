/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** suggestions hidden (shown again on demand) or deleted (never proposed
** again) for a song, found by their rule and the text they remove
*/

#include "mp3player.h"

static sug_state_t *state_find(library_t *lib, int song,
    suggest_t const *sug)
{
    for (sug_state_t *st = lib->sug_states; st; st = st->next)
        if (st->song == song && st->rule == (int)sug->rule
            && strcmp(st->key, sug->removed) == 0)
            return st;
    return NULL;
}

/*
** SUG_VISIBLE, SUG_HIDDEN or SUG_DELETED
*/
int suggest_state(library_t *lib, int song, suggest_t const *sug)
{
    sug_state_t *st = state_find(lib, song, sug);

    return st ? st->state : SUG_VISIBLE;
}

/*
** at load (rule and key come from the file) or from a choice
*/
int suggest_state_add(library_t *lib, int song, suggest_t const *sug,
    int state)
{
    sug_state_t *st = state_find(lib, song, sug);

    if (st) {
        st->state = state;
        return 0;
    }
    st = calloc(1, sizeof(sug_state_t));
    if (!st)
        return 84;
    st->key = strdup(sug->removed);
    if (!st->key) {
        free(st);
        return 84;
    }
    st->song = song;
    st->rule = sug->rule;
    st->state = state;
    st->next = lib->sug_states;
    lib->sug_states = st;
    return 0;
}

/*
** the choice is saved at once
*/
void suggest_set_state(library_t *lib, int song, suggest_t const *sug,
    int state)
{
    suggest_state_add(lib, song, sug, state);
    db_save_suggestions(lib);
}

void suggest_states_free(library_t *lib)
{
    sug_state_t *next = NULL;

    while (lib->sug_states) {
        next = lib->sug_states->next;
        free(lib->sug_states->key);
        free(lib->sug_states);
        lib->sug_states = next;
    }
}
