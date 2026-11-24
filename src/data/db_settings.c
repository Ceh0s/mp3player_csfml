/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes settings.tsv: the settings of the application, one
** per line (name, value); an unknown name is ignored
** (help, dup_remove, shuffle, repeat, volume, mute, ellipsis: see
** settings_t; next_song: see db_parse_setting)
*/

#include "mp3player.h"

/*
** the settings of the player bar
*/
static void parse_player(library_t *lib, char const *key, int value)
{
    if (strcmp(key, "shuffle") == 0)
        lib->settings.shuffle = value != 0;
    if (strcmp(key, "repeat") == 0 && value >= 0)
        lib->settings.repeat = value % 3;
    if (strcmp(key, "volume") == 0 && value >= 0)
        lib->settings.volume = value > 100 ? 100 : value;
    if (strcmp(key, "mute") == 0)
        lib->settings.mute = value != 0;
}

/*
** next_song: first id never given (an id of a forgotten song is never
** given again, see song_forget.c)
*/
void db_parse_setting(library_t *lib, char **fields, int nb)
{
    int value = 0;

    if (nb != 2)
        return;
    value = atoi(fields[1]);
    if (strcmp(fields[0], "help") == 0)
        lib->settings.help = value != 0;
    if (strcmp(fields[0], "dup_remove") == 0)
        lib->settings.dup_remove = value != 0;
    if (strcmp(fields[0], "ellipsis") == 0)
        lib->settings.ellipsis = value != 0;
    if (strcmp(fields[0], "battle_match") == 0)
        lib->settings.battle_match = value != 0;
    if (strcmp(fields[0], "links_ordered") == 0)
        lib->settings.links_ordered = value != 0;
    if (strcmp(fields[0], "next_song") == 0 && value > lib->next_song)
        lib->next_song = value;
    if (strcmp(fields[0], "next_artist") == 0 && value > lib->next_artist)
        lib->next_artist = value;
    parse_player(lib, fields[0], value);
}

/*
** the values used when settings.tsv does not give them
*/
void db_load_settings(library_t *lib)
{
    lib->settings.help = 1;
    lib->settings.volume = 100;
    lib->settings.ellipsis = 1;
    tsv_read(SETTINGS_DB, &db_parse_setting, lib);
}

int db_save_settings(library_t *lib)
{
    FILE *file = tsv_create(SETTINGS_DB, SETTINGS_COLUMNS);

    if (!file)
        return 84;
    fprintf(file, "help\t%d\n", lib->settings.help);
    fprintf(file, "dup_remove\t%d\n", lib->settings.dup_remove);
    fprintf(file, "shuffle\t%d\n", lib->settings.shuffle);
    fprintf(file, "repeat\t%d\n", lib->settings.repeat);
    fprintf(file, "volume\t%d\n", lib->settings.volume);
    fprintf(file, "mute\t%d\n", lib->settings.mute);
    fprintf(file, "ellipsis\t%d\n", lib->settings.ellipsis);
    fprintf(file, "next_song\t%d\n", lib->next_song);
    fprintf(file, "next_artist\t%d\n", lib->next_artist);
    fprintf(file, "battle_match\t%d\n", lib->settings.battle_match);
    fprintf(file, "links_ordered\t%d\n", lib->settings.links_ordered);
    return tsv_commit(file, SETTINGS_DB);
}
