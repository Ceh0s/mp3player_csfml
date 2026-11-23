/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** merges of the duplicates screen: the song kept by a click, and the
** automatic ones (an upload whose sound is the one of the version that
** YouTube generated, at DUP_AUTO % or more, goes into that version). The
** file of the merged song is removed only if the switch "Supprimer le
** fichier en trop" is on (settings.tsv); "Séparer" in the edit screen
** cancels a merge.
*/

#include "mp3player.h"

/*
** keep is kept, other is merged into it (its file removed first if
** asked: once merged, its files belong to keep)
*/
void dup_keep(app_t *app, song_t *keep, song_t *other)
{
    if (app->lib->settings.dup_remove)
        song_delete_files(app, other);
    song_merge(app, other, keep);
}

/*
** every other song of the group goes into the song i
*/
void dup_keep_all(app_t *app, dup_group_t *group, int i)
{
    song_t *keep = group->songs[i];

    for (int j = 0; j < group->nb; j++)
        if (j != i && group->songs[j])
            dup_keep(app, keep, group->songs[j]);
}

/*
** returns the number of songs merged in the group
*/
static int auto_group(app_t *app, dup_group_t *group)
{
    song_t *ref = group->songs[0];
    int merged = 0;

    if (!ref->autogen)
        return 0;
    for (int i = 1; i < group->nb; i++) {
        if (group->songs[i]->autogen || group->audio[i] < DUP_AUTO)
            continue;
        dup_keep(app, ref, group->songs[i]);
        merged++;
    }
    return merged;
}

/*
** returns the number of songs merged (the groups are cleaned after)
*/
int dup_auto_merge(app_t *app)
{
    dup_list_t *list = &app->dupscene->list;
    int merged = 0;

    for (int g = 0; g < list->nb; g++)
        merged += auto_group(app, &list->groups[g]);
    return merged;
}
