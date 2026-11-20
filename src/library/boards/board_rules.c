/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** rules of a ranking: add, remove, and their text for the screens
*/

#include "mp3player.h"

/*
** appended at the end (the order of the rules does not change the
** result); a rule already there only gets the new mode
*/
int rule_add(board_t *board, int include, rule_type_t type, int target)
{
    rule_t **last = &board->rules;
    rule_t *rule = NULL;

    for (; *last; last = &(*last)->next) {
        if ((*last)->type == type && (*last)->target == target) {
            (*last)->include = include;
            return 0;
        }
    }
    rule = calloc(1, sizeof(rule_t));
    if (!rule)
        return 84;
    rule->include = include;
    rule->type = type;
    rule->target = target;
    *last = rule;
    return 0;
}

void rule_remove(board_t *board, rule_t *rule)
{
    rule_t **cur = &board->rules;

    while (*cur && *cur != rule)
        cur = &(*cur)->next;
    if (!*cur)
        return;
    *cur = rule->next;
    free(rule);
}

static char const *target_name(library_t *lib, rule_t const *rule)
{
    playlist_t *playlist = NULL;
    artist_t *artist = NULL;
    song_t *song = NULL;

    if (rule->type == RULE_PLAYLIST) {
        playlist = playlist_by_id(lib, rule->target);
        return playlist ? playlist->name : "(playlist supprimée)";
    }
    if (rule->type == RULE_ARTIST) {
        artist = artist_by_id(lib, rule->target);
        return artist ? artist->name : "?";
    }
    song = song_resolve(lib, song_by_id(lib, rule->target));
    return song ? song_title(song) : "?";
}

/*
** "Artiste : C418" (the mode is not written)
*/
void rule_text(library_t *lib, rule_t const *rule, char *buf, int size)
{
    char const *types[NB_RULE_TYPES] = {"Toutes les musiques",
        "Playlist", "Artiste", "Titre"};

    if (rule->type == RULE_ALL) {
        snprintf(buf, size, "%s", types[RULE_ALL]);
        return;
    }
    snprintf(buf, size, "%s : %s", types[rule->type],
        target_name(lib, rule));
}

/*
** names written in board_rules.tsv
*/
char const *rule_type_name(rule_type_t type)
{
    char const *names[NB_RULE_TYPES] = {"all", "playlist", "artist",
        "song"};

    return names[type];
}

static void board_forget(board_t *board, rule_type_t type, int target)
{
    rule_t *next = NULL;

    for (rule_t *rule = board->rules; rule; rule = next) {
        next = rule->next;
        if (rule->type == type && rule->target == target)
            rule_remove(board, rule);
    }
}

/*
** removes the rules of every ranking that use this target (a playlist
** being removed)
*/
void rules_forget(library_t *lib, rule_type_t type, int target)
{
    for (board_t *board = lib->boards; board; board = board->next)
        board_forget(board, type, target);
}
