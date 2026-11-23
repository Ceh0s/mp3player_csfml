/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reads and writes likes.tsv (kind, raw id, like / dislike), in the
** order of the likes
*/

#include "mp3player.h"

void db_parse_like(library_t *lib, char **fields, int nb)
{
    int kind = 0;

    if (nb != 3)
        return;
    kind = kind_find(fields[0]);
    if (kind < 0 || atoi(fields[1]) <= 0)
        return;
    like_add(lib, kind, atoi(fields[1]),
        strcmp(fields[2], "like") == 0 ? 1 : -1);
}

int db_save_likes(library_t *lib)
{
    FILE *file = tsv_create(LIKES_DB, LIKES_COLUMNS);

    if (!file)
        return 84;
    for (like_t *like = lib->likes; like; like = like->next)
        fprintf(file, "%s\t%d\t%s\n", kind_name(like->kind), like->id,
            like->value > 0 ? "like" : "dislike");
    return tsv_commit(file, LIKES_DB);
}
