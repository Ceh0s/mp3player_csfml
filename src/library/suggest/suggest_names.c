/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** artist names written as a list in a tag ("C418, Baiyon" is one
** artist): when a suggestion names one part of such a name, the name is
** split into one artist per part, so that this part can change its role
*/

#include "mp3player.h"
#include <strings.h>

/*
** 1 if one part of the list name (separated by ',') is artist
*/
static int has_part(char const *name, char const *artist)
{
    char **parts = stwa_delimiter(name, ",", 0, 0);
    char part[NAME_LEN];
    int found = 0;

    for (int i = 0; parts && parts[i] && parts[1]; i++) {
        snprintf(part, NAME_LEN, "%s", parts[i]);
        str_trim(part);
        found = found || strcasecmp(part, artist) == 0;
    }
    str_array_free(parts);
    return found;
}

/*
** the entry i is replaced by its first part, the other parts are added
** at the end with the same role
*/
void roles_explode(artist_role_t *roles, int *nb, int i)
{
    char **parts = stwa_delimiter(roles[i].name, ",", 0, 0);
    role_t role = roles[i].role;

    for (int p = 0; parts && parts[p]; p++) {
        if (p > 0 && *nb >= EDIT_ARTISTS)
            break;
        if (p == 0) {
            snprintf(roles[i].name, NAME_LEN, "%s", parts[p]);
            str_trim(roles[i].name);
            continue;
        }
        snprintf(roles[*nb].name, NAME_LEN, "%s", parts[p]);
        str_trim(roles[*nb].name);
        roles[*nb].role = role;
        (*nb)++;
    }
    str_array_free(parts);
}

void suggest_split_names(artist_role_t *roles, int *nb, char const *artist)
{
    for (int i = 0; i < *nb; i++)
        if (has_part(roles[i].name, artist)) {
            roles_explode(roles, nb, i);
            return;
        }
}
