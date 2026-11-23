/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the YouTube videos already in music/, written before each
** queue for yt-dlp (--download-archive): a safety, the videos present
** are already left out after the listing (download_sort.c). It is
** rebuilt from the files present (their "purl" tag), so a song whose
** file was removed can be downloaded again.
*/

#include "mp3player.h"

/*
** a line of the archive is "youtube <id of 11 characters>"
*/
static void write_entries(FILE *file, library_t const *lib)
{
    char id[12];

    for (fprint_t const *fp = lib->fprints; fp; fp = fp->next)
        if (fp->path && source_video_id(fp->source, id))
            fprintf(file, "youtube %s\n", id);
    for (same_file_t const *same = lib->same_files; same; same = same->next)
        if (source_video_id(same->source, id))
            fprintf(file, "youtube %s\n", id);
}

void download_archive_write(library_t const *lib)
{
    FILE *file = fopen(DL_ARCHIVE ".tmp", "w");

    if (!file)
        return;
    write_entries(file, lib);
    fclose(file);
    rename(DL_ARCHIVE ".tmp", DL_ARCHIVE);
}
