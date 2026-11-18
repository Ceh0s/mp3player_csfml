/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scans the music folder, keeping only real mp3 files
*/

#include "mp3player.h"
#include <dirent.h>
#include <strings.h>

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

static void read_file(song_t *song, char const *name)
{
    song->file = strndup(name, strlen(name) - 4);
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
