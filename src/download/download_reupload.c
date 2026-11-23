/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a video of a listing that is no longer served ("not available": the
** old version of a song uploaded again) is looked for in the library
** under its new version, with the rules of the duplicates: same core of
** title, same version, its channel among the artists, close durations.
** The song found is added to the target playlists instead.
*/

#include "mp3player.h"

/*
** what is known of the video: the core of its title, its version, and
** its artists (channel and name before " - "); 0 if the channel is not
** known (the title alone is not enough)
*/
static int entry_info(dl_entry_t const *entry, dup_info_t *info)
{
    char prefix[NAME_LEN];
    char clean[NAME_LEN];

    if (!entry->channel || entry->channel[0] == '\0')
        return 0;
    info->song = NULL;
    dup_core(entry->title, info->core, prefix);
    dup_clean(entry->title, clean, NAME_LEN);
    info->version = dup_version(clean);
    strcpy(info->artists, "|");
    dup_names(info->artists, entry->channel);
    dup_names(info->artists, prefix);
    return info->core[0] != '\0';
}

static int matches(library_t *lib, dup_info_t const *video, int duration,
    song_t *song)
{
    dup_info_t info;
    char prefix[NAME_LEN];
    char clean[NAME_LEN];
    long diff = song->duration_ms - (long)duration * 1000;

    dup_core(song_title(song), info.core, prefix);
    if (strcmp(info.core, video->core) != 0)
        return 0;
    dup_clean(song_title(song), clean, NAME_LEN);
    if (dup_version(clean) != video->version)
        return 0;
    if (duration > 0 && song->duration_ms > 0
        && (diff > DUP_MAX_DIFF_MS || diff < -DUP_MAX_DIFF_MS))
        return 0;
    dup_artists(lib, song, prefix, info.artists);
    return dup_artists_meet(video->artists, info.artists);
}

/*
** a single song, or the single auto-generated one among them; NULL when
** nothing (or nothing sure) is found
*/
song_t *reupload_match(library_t *lib, dl_entry_t const *entry)
{
    dup_info_t video;
    song_t *found = NULL;
    song_t *autogen = NULL;
    int nb = 0;
    int nb_auto = 0;

    if (!entry_info(entry, &video))
        return NULL;
    for (int i = 0; i < lib->nb; i++) {
        if (!matches(lib, &video, entry->duration, lib->present[i]))
            continue;
        found = lib->present[i];
        nb++;
        nb_auto += found->autogen != 0;
        autogen = found->autogen ? found : autogen;
    }
    if (nb == 1)
        return found;
    return nb_auto == 1 ? autogen : NULL;
}

/*
** returns 1 if the new version was found (counted as already present)
*/
int download_reupload(app_t *app, dl_entry_t const *entry)
{
    download_t *dl = &app->download;
    song_t *song = reupload_match(app->lib, entry);
    char detail[NAME_LEN + 48];

    if (!song)
        return 0;
    playlists_append(app->lib, entry->targets, entry->nb_targets, song);
    dl->skipped++;
    snprintf(detail, sizeof(detail), "réupload : remplacé par « %s »",
        song_title(song));
    report_add(dl, REP_PRESENT, entry->title, detail);
    download_order_swap(dl, entry->id, app->lib, song);
    return 1;
}
