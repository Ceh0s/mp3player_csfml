/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** roles of the artists on a song, and their total listening time
*/

#include "mp3player.h"

long artist_listen_ms(library_t *lib, int artist_id)
{
    artist_stats_t stats;

    artist_stats(lib, artist_id, &stats);
    return stats.listen_ms;
}

char const *role_name(role_t role)
{
    char const *names[NB_ROLES] = {"main", "feat", "remix"};

    if (role < 0 || role >= NB_ROLES)
        return names[0];
    return names[role];
}

role_t role_from_name(char const *name)
{
    for (int role = 0; role < NB_ROLES; role++)
        if (strcmp(role_name(role), name) == 0)
            return role;
    return ROLE_MAIN;
}
