/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** merge of two songs: the source becomes an alias of the target, its
** stats are added to the target's ones and its files belong to it.
** Nothing is copied, so a merge can always be undone (song_split).
*/

#include "mp3player.h"

static void log_merge(char const *action, int source, int target)
{
    char line[64];

    snprintf(line, sizeof(line), "%s\t%d\t%d", action, source, target);
    log_line(MERGES_LOG, MERGES_COLUMNS, line);
}

int song_merge(app_t *app, song_t *source, song_t *target)
{
    if (!source || !target || source == target || source->merged_into
        || target->merged_into)
        return 84;
    if (app->player->song == source)
        player_stop(app, END_SKIP);
    source->merged_into = target->id;
    log_merge("merge", source->id, target->id);
    library_changed(app);
    return 0;
}

void song_split(app_t *app, song_t *alias)
{
    if (!alias->merged_into)
        return;
    log_merge("split", alias->id, alias->merged_into);
    alias->merged_into = 0;
    library_changed(app);
}

/*
** 1 if both titles share a word of 3 letters or more
*/
static int share_word(char const *a, char const *b)
{
    char *low_a = str_lower(a);
    char *low_b = str_lower(b);
    char *word = NULL;
    char *save = NULL;
    int found = 0;

    if (!low_a || !low_b) {
        free(low_a);
        free(low_b);
        return 1;
    }
    word = strtok_r(low_a, " -_()[].,'", &save);
    for (; word && !found; word = strtok_r(NULL, " -_()[].,'", &save))
        found = strlen(word) >= 3 && strstr(low_b, word) != NULL;
    free(low_a);
    free(low_b);
    return found;
}

/*
** fills warn with the reasons to double check the merge,
** returns their number
*/
static int duration_warning(song_t const *src, song_t const *dst,
    char *warn)
{
    long diff = src->duration_ms - dst->duration_ms;
    char a[16];
    char b[16];

    if ((diff <= 5000 && diff >= -5000) || src->duration_ms <= 0
        || dst->duration_ms <= 0)
        return 0;
    format_time(a, sizeof(a), src->duration_ms);
    format_time(b, sizeof(b), dst->duration_ms);
    snprintf(warn, 128, "Durées différentes : %s contre %s", a, b);
    return 1;
}

int merge_warnings(song_t const *src, song_t const *dst,
    char warn[EDIT_WARNINGS][128])
{
    int nb = duration_warning(src, dst, warn[0]);

    if (!share_word(song_title(src), song_title(dst))) {
        snprintf(warn[nb], 128, "Aucun mot en commun dans les titres");
        nb++;
    }
    if (src->present && dst->present) {
        snprintf(warn[nb], 128, "Les deux fichiers sont dans music/ : "
            "un seul restera dans la liste");
        nb++;
    }
    return nb;
}
