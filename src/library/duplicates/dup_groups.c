/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** gathers the pairs of possible duplicates in groups: two pairs holding
** a same song are one group (a song downloaded three times is one
** group of three, even if the first and the last were not paired)
*/

#include "mp3player.h"

static int has_song(dup_group_t const *group, song_t const *song)
{
    for (int i = 0; i < group->nb; i++)
        if (group->songs[i] == song)
            return 1;
    return 0;
}

static int group_of(dup_list_t const *list, song_t const *song)
{
    for (int g = 0; g < list->nb; g++)
        if (has_song(&list->groups[g], song))
            return g;
    return -1;
}

static void add_song(dup_group_t *group, song_t *song)
{
    if (group->nb >= DUP_GROUP_MAX)
        return;
    group->songs[group->nb] = song;
    group->audio[group->nb] = -1;
    group->nb++;
}

/*
** the songs of the group from go into the group to, from is replaced
** by the last group
*/
static void join(dup_list_t *list, int to, int from)
{
    dup_group_t *src = &list->groups[from];

    for (int i = 0; i < src->nb; i++)
        add_song(&list->groups[to], src->songs[i]);
    list->nb--;
    list->groups[from] = list->groups[list->nb];
}

static void add_pair(dup_list_t *list, dup_pair_t const *pair)
{
    int ga = group_of(list, pair->a);
    int gb = group_of(list, pair->b);

    if (ga >= 0 && gb >= 0 && ga != gb)
        join(list, ga, gb);
    if (ga >= 0 && gb < 0)
        add_song(&list->groups[ga], pair->b);
    if (gb >= 0 && ga < 0)
        add_song(&list->groups[gb], pair->a);
    if (ga >= 0 || gb >= 0)
        return;
    list->groups[list->nb].nb = 0;
    add_song(&list->groups[list->nb], pair->a);
    add_song(&list->groups[list->nb], pair->b);
    list->nb++;
}

/*
** there are at most as many groups as pairs; the pairs are freed;
** returns 84 on a failed allocation
*/
int dup_group_pairs(dup_list_t *list)
{
    list->groups = malloc(sizeof(dup_group_t) * (list->nb_pairs + 1));
    list->nb = 0;
    if (list->groups)
        for (int i = 0; i < list->nb_pairs; i++)
            add_pair(list, &list->pairs[i]);
    free(list->pairs);
    list->pairs = NULL;
    list->nb_pairs = 0;
    return list->groups ? 0 : 84;
}

/*
** takes the song i out of the group
*/
void dup_group_remove(dup_group_t *group, int i)
{
    for (int j = i; j + 1 < group->nb; j++) {
        group->songs[j] = group->songs[j + 1];
        group->audio[j] = group->audio[j + 1];
    }
    group->nb--;
}
