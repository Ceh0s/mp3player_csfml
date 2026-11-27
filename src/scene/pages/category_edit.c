/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** window of a category, over the screen (opened by a right click in the
** list of the categories or on the stats of a page): its description and
** what it measures; for a category of the author, its name too, and
** "Supprimer" (two clicks); also the window of a new category
*/

#include "mp3player.h"

static const sfColor gray = {190, 190, 190, 255};

/*
** stat : the stat shown, CAT_NEW - 1 for a new category
*/
void catdesc_open(app_t *app, int stat)
{
    catlist_t *list = &app->catlist;

    list->edit = stat + 1;
    list->confirm = 0;
    list->refused = 0;
    list->open = 0;
    if (list->edit == CAT_NEW) {
        list->name[0] = '\0';
        list->text[0] = '\0';
        list->flaw = 0;
        list->board_flaw = 0;
        list->field = 1;
        return;
    }
    snprintf(list->name, CAT_NAME_LEN, "%s", app->lib->cats[stat].name);
    snprintf(list->text, DESC_LEN, "%s", app->lib->cats[stat].desc);
    list->flaw = app->lib->cats[stat].flaw;
    list->board_flaw = category_flaw(app->lib, app->arena.board, stat);
    list->field = 0;
}

/*
** 1 if the name can be typed: a new category or one of the author
*/
static int named(app_t *app)
{
    int edit = app->catlist.edit;

    return edit == CAT_NEW || app->lib->cats[edit - 1].custom;
}

/*
** the new category, or the name of one of the author; returns its index,
** -1 if the name is refused
*/
static int save_name(app_t *app)
{
    catlist_t *list = &app->catlist;
    int stat = list->edit - 1;

    if (list->edit == CAT_NEW)
        return category_add(app->lib, list->name, "", 0);
    if (app->lib->cats[stat].custom
        && category_rename(app->lib, stat, list->name) != 0)
        return -1;
    return stat;
}

/*
** returns 2 once saved, 0 if the name is refused (the window stays)
*/
int catdesc_save(app_t *app)
{
    catlist_t *list = &app->catlist;
    int stat = save_name(app);

    if (stat < 0) {
        list->refused = 1;
        return 0;
    }
    category_set_desc(app->lib, stat, list->text);
    app->lib->cats[stat].flaw = list->flaw;
    board_flaw_set(app->lib, app->arena.board, stat, list->board_flaw);
    db_save_categories(app->lib);
    db_save_board_flaws(app->lib);
    list->edit = 0;
    return 2;
}

/*
** the categories chosen on the screens follow the removal
*/
static int remove_category(app_t *app)
{
    catlist_t *list = &app->catlist;
    int stat = list->edit - 1;
    int *elo = &app->stats_opt.elo;

    if (!list->confirm) {
        list->confirm = 1;
        return 0;
    }
    if (category_remove(app->lib, stat) != 0)
        return 0;
    app->arena.category = category_after_remove(app->arena.category, stat);
    if (*elo > 0)
        *elo = category_after_remove(*elo - 1, stat) + 1;
    list->edit = 0;
    return 3;
}

/*
** the ranking that follows the category follows it here too
*/
static void measure_button(app_t *app, float x, float y)
{
    float rect[4] = {x, y, 380, 52};
    catlist_t *list = &app->catlist;

    if (!widget_button(app, rect, list->flaw ? "Mesure : un défaut"
            : "Mesure : une qualité"))
        return;
    if (list->board_flaw == list->flaw)
        list->board_flaw = !list->board_flaw;
    list->flaw = !list->flaw;
}

/*
** what it measures in the ranking of the battles and rankings screens
** (the one of a playlist, or a custom one); none for "Général"
*/
static void board_button(app_t *app, float x, float y)
{
    catlist_t *list = &app->catlist;
    board_t *board = board_by_id(app->lib, app->arena.board);
    char label[NAME_LEN + 48];

    if (!board || board->id == 0)
        return;
    snprintf(label, sizeof(label), "Dans « %s » : %s", board->name,
        list->board_flaw ? "un défaut" : "une qualité");
    if (widget_button(app, (float [4]){x, y, 740, 52}, label))
        list->board_flaw = !list->board_flaw;
}

/*
** 60 more pixels when a ranking other than "Général" is chosen
*/
static float board_row(app_t *app)
{
    return app->arena.board != 0
        && board_by_id(app->lib, app->arena.board) ? 60 : 0;
}

/*
** the name (a field for a category of the author), then the description;
** a click on a field gives it the keys
*/
static void fields(app_t *app)
{
    catlist_t *list = &app->catlist;

    if (!named(app)) {
        widget_text(app, list->name, (float [3]){590, 320, 32}, &sfWhite);
    } else {
        widget_text(app, "Nom", (float [3]){590, 318, 20}, &gray);
        if (widget_field(app, (float [4]){700, 312, 630, 52}, list->name,
                list->field))
            list->field = 1;
    }
    widget_text(app, "Description", (float [3]){590, 394, 20}, &gray);
    if (widget_field(app, (float [4]){700, 388, 630, 52}, list->text,
            !list->field))
        list->field = 0;
    if (list->refused)
        widget_text(app, "Nom vide ou déjà pris.",
            (float [3]){590, 452, 20}, &gray);
}

/*
** returns 2 once saved, 3 once removed
*/
static int buttons(app_t *app)
{
    catlist_t *list = &app->catlist;
    float row = board_row(app);
    float trash[4] = {870, 570 + row, 360, 56};

    measure_button(app, 590, 486);
    board_button(app, 590, 546);
    if (widget_icon_button(app, (float [4]){590, 570 + row, 260, 56},
            ICON_SAVE,
            "Enregistrer"))
        return catdesc_save(app);
    if (list->edit != CAT_NEW && named(app)
        && widget_icon_button(app, trash, ICON_TRASH, list->confirm
            ? "Confirmer la suppression" : "Supprimer"))
        return remove_category(app);
    return 0;
}

/*
** returns 2 once saved, 3 once a category was removed (a flaw counts the
** other way in TOTAL). Called first when clicking (it takes the click),
** last when drawing (it is above the screen).
*/
int catdesc_view(app_t *app)
{
    catlist_t *list = &app->catlist;
    sfFloatRect view = app->view_rect;
    float win[4] = {560, 290, 800, 370 + board_row(app)};
    sfColor shade = {0, 0, 0, 170};
    sfColor back = {30, 30, 30, 245};
    int result = 0;

    if (!list->edit)
        return 0;
    widget_rect(app, (float [4]){view.left, view.top, view.width,
        view.height}, &shade);
    widget_rect(app, win, &back);
    fields(app);
    result = buttons(app);
    if (result == 0 && widget_icon_tip(app, (float [4]){1372, 290, 48, 48},
            ICON_CLOSE, "Fermer"))
        list->edit = 0;
    if (app->wg->mode == WG_CLICK)
        app->wg->mode = WG_IDLE;
    return result;
}
