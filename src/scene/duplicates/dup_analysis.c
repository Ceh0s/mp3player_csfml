/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** sound of the possible duplicates: the songs of the pairs whose sound
** is not known yet are read one per tick (a file takes a fraction of a
** second to decode), then each pair is compared. The pairs whose sound
** differs are left out (two different songs with the same title), the
** others are sorted, the surest first.
*/

#include "mp3player.h"

static void add_todo(app_t *app, dupscene_t *scene, song_t *song)
{
    for (int i = 0; i < scene->nb_todo; i++)
        if (scene->todo[i] == song)
            return;
    if (envelope_find(app->lib, song_hash(app->lib, song)))
        return;
    scene->todo[scene->nb_todo] = song;
    scene->nb_todo++;
}

int dupscene_todo(app_t *app)
{
    dupscene_t *scene = app->dupscene;

    scene->todo = malloc(sizeof(song_t *) * (scene->list.nb * 2 + 1));
    if (!scene->todo)
        return 84;
    for (int i = 0; i < scene->list.nb; i++) {
        add_todo(app, scene, scene->list.pairs[i].a);
        add_todo(app, scene, scene->list.pairs[i].b);
    }
    return 0;
}

static void compare_pair(library_t *lib, dup_pair_t *pair)
{
    envelope_t *a = song_envelope(lib, pair->a);
    envelope_t *b = song_envelope(lib, pair->b);

    pair->audio = -1;
    if (a && b && a->nb > 0 && b->nb > 0)
        pair->audio = envelope_compare(a, b);
}

/*
** insertion sort (a few pairs): the highest likeness first, the pairs
** that could not be compared last
*/
static void sort_pairs(dup_list_t *list)
{
    dup_pair_t pair;
    int j = 0;

    for (int i = 1; i < list->nb; i++) {
        pair = list->pairs[i];
        j = i;
        while (j > 0 && list->pairs[j - 1].audio < pair.audio) {
            list->pairs[j] = list->pairs[j - 1];
            j--;
        }
        list->pairs[j] = pair;
    }
}

static void finish(app_t *app, dupscene_t *scene)
{
    int kept = 0;

    for (int i = 0; i < scene->list.nb; i++) {
        compare_pair(app->lib, &scene->list.pairs[i]);
        if (scene->list.pairs[i].audio >= 0
            && scene->list.pairs[i].audio < DUP_DIFFERENT)
            continue;
        scene->list.pairs[kept] = scene->list.pairs[i];
        kept++;
    }
    scene->list.nb = kept;
    sort_pairs(&scene->list);
    scene->ready = 1;
}

void dupscene_update(app_t *app)
{
    dupscene_t *scene = app->dupscene;

    if (scene->ready)
        return;
    if (scene->done < scene->nb_todo) {
        song_envelope(app->lib, scene->todo[scene->done]);
        scene->done++;
        return;
    }
    finish(app, scene);
}

/*
** after a choice: the pairs decided (a = NULL), or holding a song merged
** since, or no longer present, are taken out
*/
void dupscene_clean(app_t *app)
{
    dup_list_t *list = &app->dupscene->list;
    dup_pair_t *pair = NULL;
    int kept = 0;

    for (int i = 0; i < list->nb; i++) {
        pair = &list->pairs[i];
        if (!pair->a || pair->a->merged_into || pair->b->merged_into
            || !pair->a->present || !pair->b->present)
            continue;
        list->pairs[kept] = *pair;
        kept++;
    }
    list->nb = kept;
    dupscene_scroll(app, 0);
}
