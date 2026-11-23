/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes settings.tsv: the settings of the application, one
** per line (name, value); an unknown name is ignored
** (help, dup_remove, shuffle, repeat: see settings_t)
*/

#include "mp3player.h"

void db_parse_setting(library_t *lib, char **fields, int nb)
{
    if (nb != 2)
        return;
    if (strcmp(fields[0], "help") == 0)
        lib->settings.help = atoi(fields[1]) != 0;
    if (strcmp(fields[0], "dup_remove") == 0)
        lib->settings.dup_remove = atoi(fields[1]) != 0;
    if (strcmp(fields[0], "shuffle") == 0)
        lib->settings.shuffle = atoi(fields[1]) != 0;
    if (strcmp(fields[0], "repeat") == 0 && atoi(fields[1]) >= 0)
        lib->settings.repeat = atoi(fields[1]) % 3;
}

/*
** the values used when settings.tsv does not give them
*/
void db_load_settings(library_t *lib)
{
    lib->settings.help = 1;
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
    return tsv_commit(file, SETTINGS_DB);
}
