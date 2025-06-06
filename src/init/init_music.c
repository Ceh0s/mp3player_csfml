/*
** EPITECH PROJECT, 2025
** init music
** File description:
** initialize all music related elements
*/

#include "../../include/mp3player.h"

static int strstr_bool(char *str, char const *to_find)
{
    int x = 0;
    int len = strlen(to_find);

    if (to_find == NULL)
        return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        while (str[i] == to_find[x] && to_find[x] != '\0') {
            i++;
            x++;
        }
        if (x == len)
            return 1;
        if (x != 0)
            i--;
        x = 0;
    }
    return 0;
}

static int search_music(void)
{
    DIR *dir = opendir("music");
    struct dirent *dirent;
    int i = 0;

    if (!dir) {
        closedir(dir);
        return i;
    }
    dirent = readdir(dir);
    while (dirent) {
        if (strstr_bool(dirent->d_name, ".mp3"))
            i++;
        dirent = readdir(dir);
    }
    closedir(dir);
    return i;
}

static char **send_array(char **arr)
{
    DIR *dir = opendir("music");
    struct dirent *dirent;
    int i = 0;

    if (!dir) {
        closedir(dir);
	return NULL;
    }
    dirent = readdir(dir);
    while (dirent) {
        if (strstr_bool(dirent->d_name, ".mp3") == 1) {
            arr[i] = strdup(dirent->d_name);
            i++;
	}
        dirent = readdir(dir);
    }
    closedir(dir);
    arr[i] = NULL;
    return arr;
}

static char **music_to_array(music_t **music)
{
    char **arr = NULL;
    int i = search_music();

    (*music)->nb = i;
    if (i == 0)
	return arr;
    arr = malloc(sizeof(char *) * (i + 1));
    arr = send_array(arr);
    return arr;
}

static music_t *init_text(music_t *music, window_t *win)
{
    int i = 0;
    int offset = win->width / 68;
    char *str = NULL;

    music->text = malloc(sizeof(sfText *) * (music->nb + 1));
    for (; music->music[i]; i++) {
        str = remove_mp3_tag(music->music[i]);
        music->text[i] = sfText_create();
        sfText_setFont(music->text[i], music->font);
        sfText_setString(music->text[i], str);
        sfText_setCharacterSize(music->text[i], win->width / 55);
        sfText_setFillColor(music->text[i], sfWhite);
        sfText_setPosition(music->text[i],
        (sfVector2f){((float)win->width / 15),
        ((float)(win->width / 15) + offset)});
        offset += win->width / 54;
        free(str);
    }
    music->text[i] = NULL;
    return music;
}

static music_t *create_music(music_t *music)
{
    int i = 0;
    char *path = NULL;

    music->mus = malloc(sizeof(sfMusic *) * (music->nb + 1));
    for (; music->music[i]; i++) {
        path = malloc(sizeof(char) *(strlen(music->music[i]) + 7));
        sprintf(path, "music/%s", music->music[i]);
        music->mus[i] = sfMusic_createFromFile(path);
        free(path);
    }
    music->mus[i] = NULL;
    return music;
}

music_t *init_music(window_t *win)
{
    music_t *music = malloc(sizeof(music_t));

    music->music = music_to_array(&music);
    music->font = sfFont_createFromFile(
    "./src/fonts/BourgeoisW00Medium.ttf");
    if (music->nb > 0) {
        music = init_text(music, win);
        music = create_music(music);
    }
    music->paused = 1;
    return music;
}
