/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes settings.tsv: the settings of the application, one
** per line (name, value); an unknown name is ignored
** dl_min / dl_max : duration range of the download screen (seconds, -1 =
** no limit)
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
    if (strcmp(fields[0], "dl_min") == 0)
        lib->settings.dl_min_s = atoi(fields[1]);
    if (strcmp(fields[0], "dl_max") == 0)
        lib->settings.dl_max_s = atoi(fields[1]);
}

/*
** the values used when settings.tsv does not give them
*/
void db_load_settings(library_t *lib)
{
    lib->settings.help = 1;
    lib->settings.dl_min_s = -1;
    lib->settings.dl_max_s = -1;
    tsv_read(SETTINGS_DB, &db_parse_setting, lib);
}

int db_save_settings(library_t *lib)
{
    FILE *file = tsv_create(SETTINGS_DB, SETTINGS_COLUMNS);

    if (!file)
        return 84;
    fprintf(file, "help\t%d\n", lib->settings.help);
    fprintf(file, "dup_remove\t%d\n", lib->settings.dup_remove);
    fprintf(file, "dl_min\t%d\n", lib->settings.dl_min_s);
    fprintf(file, "dl_max\t%d\n", lib->settings.dl_max_s);
    return tsv_commit(file, SETTINGS_DB);
}
