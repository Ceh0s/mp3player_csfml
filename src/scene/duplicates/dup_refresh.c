/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** after a choice in the duplicates screen: the songs taken out of their
** group (NULL), merged since or no longer present leave it, a group of
** fewer than two songs is gone, and the groups are sorted again
*/

#include "mp3player.h"

/*
** the likeness of the song closest to the reference
*/
static int best(dup_group_t const *group)
{
    int found = -1;

    for (int i = 1; i < group->nb; i++)
        if (group->audio[i] > found)
            found = group->audio[i];
    return found;
}

/*
** insertion sort (a few groups): the highest likeness first, the
** groups that could not be compared last
*/
static void sort_groups(dup_list_t *list)
{
    dup_group_t group;
    int j = 0;

    for (int i = 1; i < list->nb; i++) {
        group = list->groups[i];
        j = i;
        while (j > 0 && best(&list->groups[j - 1]) < best(&group)) {
            list->groups[j] = list->groups[j - 1];
            j--;
        }
        list->groups[j] = group;
    }
}

static void clean_group(dup_group_t *group)
{
    song_t *song = NULL;

    for (int i = group->nb - 1; i >= 0; i--) {
        song = group->songs[i];
        if (!song || song->merged_into || !song->present)
            dup_group_remove(group, i);
    }
}

void dupscene_clean(app_t *app)
{
    dup_list_t *list = &app->dupscene->list;
    int kept = 0;

    for (int g = 0; g < list->nb; g++) {
        clean_group(&list->groups[g]);
        if (list->groups[g].nb < 2)
            continue;
        list->groups[kept] = list->groups[g];
        kept++;
    }
    list->nb = kept;
    sort_groups(list);
    dupscene_scroll(app, 0);
}
