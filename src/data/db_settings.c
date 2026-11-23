/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes settings.tsv: the settings of the application, one
** per line (name, value); an unknown name is ignored
*/

#include "mp3player.h"

void db_parse_setting(library_t *lib, char **fields, int nb)
{
    if (nb != 2)
        return;
    if (strcmp(fields[0], "help") == 0)
        lib->settings.help = atoi(fields[1]) != 0;
}

int db_save_settings(library_t *lib)
{
    FILE *file = tsv_create(SETTINGS_DB, SETTINGS_COLUMNS);

    if (!file)
        return 84;
    fprintf(file, "help\t%d\n", lib->settings.help);
    return tsv_commit(file, SETTINGS_DB);
}
