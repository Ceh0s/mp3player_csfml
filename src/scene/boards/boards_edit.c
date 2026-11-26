/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** edition of a ranking: its name, its rules (include / exclude, remove)
** and its removal; the general ranking is only shown
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

/*
** number of songs in the ranking, counted again after each change
*/
void boards_count(app_t *app)
{
    arena_t arena = {KIND_SONG, app->boards->board->id, CAT_ALL};
    pool_t pool = {NULL, 0};

    pool_build(app->lib, &arena, &pool);
    app->boards->members = pool.nb;
    free(pool.items);
}

static void header(app_t *app, boards_t *bd)
{
    char line[64];

    widget_text(app, "Classement", (float [3]){130, 115, 40}, &sfWhite);
    if (bd->board->id == BOARD_GENERAL) {
        widget_text(app, bd->board->name, (float [3]){130, 200, 36},
            &sfWhite);
        widget_text(app, "Toutes les musiques, non modifiable.",
            (float [3]){130, 278, 22}, &gray);
        return;
    }
    widget_text(app, "Nom", (float [3]){130, 172, 24}, &sfWhite);
    if (widget_field(app, (float [4]){130, 205, 900, 56}, bd->name,
            bd->focus == 0))
        bd->focus = 0;
    snprintf(line, sizeof(line), "%d musique%s",
        bd->members, bd->members > 1 ? "s" : "");
    widget_text(app, line, (float [3]){130, 278, 22}, &gray);
}

/*
** returns 1 if the rule was removed (the list must not be read further)
*/
static int rule_row(app_t *app, boards_t *bd, rule_t *rule, float y)
{
    char text[NAME_LEN + 32];
    int fixed = bd->board->id == BOARD_GENERAL;

    rule_text(app->lib, rule, text, sizeof(text));
    if (!fixed && widget_button(app, (float [4]){130, y, 170, 48},
            rule->include ? "Inclure" : "Exclure")) {
        rule->include = !rule->include;
        boards_count(app);
    }
    widget_text(app, text, (float [3]){320, y + 8, 26}, &sfWhite);
    if (fixed || !widget_button(app, (float [4]){900, y, 160, 48},
            "Retirer"))
        return 0;
    rule_remove(bd->board, rule);
    boards_count(app);
    return 1;
}

/*
** the rules, left of the column adding them; they scroll
*/
static void rules(app_t *app, boards_t *bd)
{
    float top = BOARDS_ROWS_Y + 46;
    float area[4] = {120, top, 940, CONTENT_BOTTOM + app->extra_h - top};
    int nb = 0;
    float y = 0;

    widget_text(app, "Une exclusion l'emporte sur une inclusion.",
        (float [3]){130, BOARDS_ROWS_Y, 22}, &gray);
    for (rule_t *rule = bd->board->rules; rule; rule = rule->next)
        nb++;
    scroll_frame(&bd->list, area, nb * 56);
    scroll_bar_draw(app, &bd->list);
    if (!scroll_rows_begin(app, &bd->list))
        return;
    nb = 0;
    for (rule_t *rule = bd->board->rules; rule; rule = rule->next) {
        y = scroll_y(&bd->list, nb * 56 + 4);
        nb++;
        if (scroll_seen(&bd->list, y, 48) && rule_row(app, bd, rule, y))
            break;
    }
    scroll_rows_end(app);
}

void boards_edit(app_t *app)
{
    boards_t *bd = app->boards;
    float y = 840 + app->extra_h;

    header(app, bd);
    rules(app, bd);
    if (bd->board->id != BOARD_GENERAL)
        boards_add_column(app);
    if (widget_back(app)) {
        boards_leave(app);
        return;
    }
    if (bd->board->id != BOARD_GENERAL && widget_icon_button(app,
            (float [4]){1380, y, 400, 56}, ICON_TRASH,
            "Supprimer le classement"))
        bd->mode = BOARDS_CONFIRM;
}

/*
** its battles stay in battles.log but are ignored from now on
*/
void boards_confirm(app_t *app)
{
    boards_t *bd = app->boards;
    char line[NAME_LEN + 64];
    float y = 840 + app->extra_h;

    snprintf(line, sizeof(line), "Supprimer le classement « %s » ?",
        bd->board->name);
    widget_text(app, line, (float [3]){130, 115, 40}, &sfWhite);
    widget_text(app, "Ses battles ne compteront plus.",
        (float [3]){130, 190, 24}, &gray);
    if (widget_button(app, (float [4]){130, y, 260, 56}, "Supprimer")) {
        board_remove(app->lib, bd->board);
        bd->board = NULL;
        bd->mode = BOARDS_LIST;
        app->arena.board = BOARD_GENERAL;
        db_load_elo(app->lib);
        return;
    }
    if (widget_button(app, (float [4]){410, y, 260, 56}, "Annuler"))
        bd->mode = BOARDS_EDIT;
}
