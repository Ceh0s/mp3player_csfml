/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** adding a rule to a ranking: every song, or a playlist, an artist or
** a song found by what is typed (to include or to exclude)
*/

#include "mp3player.h"

static void add_result(boards_t *bd, rule_type_t type, int target)
{
    if (bd->nb_results >= BOARD_RESULTS)
        return;
    bd->results[bd->nb_results].type = type;
    bd->results[bd->nb_results].target = target;
    bd->results[bd->nb_results].next = NULL;
    bd->nb_results++;
}

/*
** playlists first, then artists, then songs (not the merged ones)
*/
static void search(app_t *app, boards_t *bd)
{
    bd->nb_results = 0;
    if (bd->search[0] == '\0')
        return;
    for (playlist_t *pl = app->lib->playlists; pl; pl = pl->next)
        if (str_contains(pl->name, bd->search))
            add_result(bd, RULE_PLAYLIST, pl->id);
    for (artist_t *a = app->lib->artists; a; a = a->next)
        if (str_contains(a->name, bd->search)
            && artist_linked(app->lib, a->id))
            add_result(bd, RULE_ARTIST, a->id);
    for (song_t *s = app->lib->songs; s; s = s->next)
        if (!s->merged_into && str_contains(song_title(s), bd->search))
            add_result(bd, RULE_SONG, s->id);
}

/*
** 9 = tab: switches between the name and the search fields
*/
void boards_text(app_t *app, sfUint32 code)
{
    boards_t *bd = app->boards;
    char *field = NULL;

    if (bd->mode != BOARDS_EDIT || bd->board->id == BOARD_GENERAL)
        return;
    if (code == 9) {
        bd->focus = !bd->focus;
        return;
    }
    field = bd->focus ? bd->search : bd->name;
    if (text_input(app, field, code) && bd->focus)
        search(app, bd);
}

static void add(app_t *app, rule_t const *result, int include)
{
    rule_add(app->boards->board, include, result->type, result->target);
    boards_count(app);
}

static void result_rows(app_t *app, boards_t *bd)
{
    char text[NAME_LEN + 32];
    float y = 0;

    for (int i = 0; i < bd->nb_results; i++) {
        y = BOARDS_ROWS_Y + 50 + i * 56;
        rule_text(app->lib, &bd->results[i], text, sizeof(text));
        widget_text(app, text, (float [3]){1100, y + 10, 22}, &sfWhite);
        if (widget_button(app, (float [4]){1500, y, 135, 48}, "Inclure"))
            add(app, &bd->results[i], 1);
        if (widget_button(app, (float [4]){1645, y, 135, 48}, "Exclure"))
            add(app, &bd->results[i], 0);
    }
}

void boards_add_column(app_t *app)
{
    boards_t *bd = app->boards;

    widget_text(app, "Ajouter une règle", (float [3]){1100, 172, 24},
        &sfWhite);
    if (widget_field(app, (float [4]){1100, 205, 680, 56}, bd->search,
            bd->focus == 1))
        bd->focus = 1;
    if (widget_button(app, (float [4]){1100, 275, 680, 48},
            "+ Toutes les musiques"))
        add(app, &(rule_t){1, RULE_ALL, 0, NULL}, 1);
    result_rows(app, bd);
}
