/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scans the music folder, keeping only real mp3 files
*/

#include "mp3player.h"
#include <dirent.h>
#include <strings.h>
#include <ctype.h>

static int has_mp3_ext(char const *name)
{
    size_t len = strlen(name);

    return len > 4 && strcasecmp(name + len - 4, ".mp3") == 0;
}

static char *join_path(char const *dir, char const *name)
{
    char *path = malloc(strlen(dir) + strlen(name) + 2);

    if (path)
        sprintf(path, "%s/%s", dir, name);
    return path;
}

/*
** a downloaded file is named "title [id].mp3" (two videos with the same
** title never replace each other): the id of a YouTube video (11
** letters, digits, - or _) is not part of the name shown
*/
static size_t shown_length(char const *name)
{
    size_t len = strlen(name) - 4;
    char const *id = name + len - 12;

    if (len < 15 || name[len - 1] != ']' || strncmp(id - 2, " [", 2) != 0)
        return len;
    for (int i = 0; i < 11; i++)
        if (!isalnum((unsigned char)id[i]) && id[i] != '-' && id[i] != '_')
            return len;
    return len - 14;
}

static void read_file(song_t *song, char const *name)
{
    song->file = strndup(name, shown_length(name));
    id3_read(song, song->path);
    song->key = song_make_key(song);
    song->hash = fingerprint_file(song->path);
}

static song_t *load_song(char const *dir, char const *name)
{
    song_t *song = NULL;
    char *path = join_path(dir, name);

    if (!path || !mp3_is_valid(path)) {
        fprintf(stderr, "Ignored (not a real mp3): %s/%s\n", dir, name);
        free(path);
        return NULL;
    }
    song = song_create();
    if (!song) {
        free(path);
        return NULL;
    }
    song->path = path;
    read_file(song, name);
    return song;
}

static song_t *add_entry(song_t *list, char const *dir, char const *name)
{
    song_t *song = NULL;

    if (name[0] == '.' || !has_mp3_ext(name))
        return list;
    song = load_song(dir, name);
    if (!song || !song->key || !song->file || !song->hash) {
        song_destroy(song);
        return list;
    }
    song->next = list;
    return song;
}

/*
** a single file of the folder (just downloaded), NULL if not a real mp3
*/
song_t *scan_file(char const *dir, char const *name)
{
    return add_entry(NULL, dir, name);
}

song_t *scan_music(char const *dir)
{
    DIR *folder = opendir(dir);
    struct dirent *entry = NULL;
    song_t *list = NULL;

    if (!folder) {
        fprintf(stderr, "Cannot open the '%s' folder\n", dir);
        return NULL;
    }
    for (entry = readdir(folder); entry; entry = readdir(folder))
        list = add_entry(list, dir, entry->d_name);
    closedir(folder);
    return list;
}
