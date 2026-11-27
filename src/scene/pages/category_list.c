/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the categories opened by the category bar (category_bar.c):
** a click chooses one, its box counts it in TOTAL or not, the mouse
** over it shows its description, a right click opens the window of the
** category (category_edit.c, also opened from the stats of a page); its
** last row creates a category. A category dragged to another row takes
** its place (category_order.c). A second column when it is too long.
*/

#include "mp3player.h"

static const sfColor gray = {190, 190, 190, 255};

    #define LIST_ROW 44
    #define LIST_COL 490
    #define ROWS_PER_COL 16
    #define ROW_NEW -1

static void row_rect(int row, float *rect)
{
    rect[0] = 136 + row / ROWS_PER_COL * LIST_COL;
    rect[1] = CAT_BAR_Y + 66 + row % ROWS_PER_COL * LIST_ROW;
    rect[2] = 478;
    rect[3] = LIST_ROW - 4;
}

/*
** the categories, then TOTAL (in the rankings), then the row creating
** one (ROW_NEW) if there is room
*/
static int row_stat(app_t *app, int row)
{
    int nb = app->lib->nb_cats;

    if (row < nb)
        return stat_at(app->lib, row);
    if (row == nb && app->catlist.total)
        return CAT_TOTAL;
    return ROW_NEW;
}

static int rows_count(app_t *app)
{
    int nb = app->lib->nb_cats + app->catlist.total;

    if (app->lib->nb_cats < MAX_CATEGORIES)
        nb++;
    return nb;
}

/*
** row under the mouse, -1 if none
*/
static int row_at(app_t *app)
{
    float rect[4];

    for (int row = 0; row < app->catlist.nb; row++) {
        row_rect(row, rect);
        if (mouse_in_rect(app, rect))
            return row;
    }
    return -1;
}

/*
** a category pressed then released on another row goes there; returns 1
** if it moved (the release is not a click)
*/
static int drop(app_t *app, sfEvent *event)
{
    catlist_t *list = &app->catlist;
    int from = list->drag - 1;
    int to = row_at(app);

    if (event->type == sfEvtMouseButtonPressed && !list->edit) {
        list->drag = to >= 0 && to < app->lib->nb_cats ? to + 1 : 0;
        return 0;
    }
    list->drag = 0;
    if (from < 0 || to < 0 || to >= app->lib->nb_cats || to == from)
        return 0;
    category_place(app->lib, stat_at(app->lib, from), to);
    db_save_categories(app->lib);
    return 1;
}

/*
** while the list or the window is open, it takes the keys, the wheel and
** the right clicks (returns 1); the left clicks go through the view of
** the scene, which gives them to the list first, unless one dragged a
** category
*/
int catlist_event(app_t *app, sfEvent *event)
{
    catlist_t *list = &app->catlist;
    int row = 0;

    if (!list->open && !list->edit)
        return 0;
    if ((event->type == sfEvtMouseButtonPressed
        || event->type == sfEvtMouseButtonReleased)
        && event->mouseButton.button == sfMouseLeft && drop(app, event))
        return 1;
    if (event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseLeft)
        return 0;
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyEscape) {
        if (list->edit)
            list->edit = 0;
        else
            list->open = 0;
    }
    if (event->type == sfEvtTextEntered && list->edit) {
        if (event->text.unicode == 13)
            catdesc_save(app);
        else if (list->field)
            text_input_n(app, list->name, CAT_NAME_LEN, event->text.unicode);
        else
            text_input_n(app, list->text, DESC_LEN, event->text.unicode);
    }
    if (event->type == sfEvtMouseButtonPressed && !list->edit
        && event->mouseButton.button == sfMouseRight) {
        row = row_at(app);
        if (row >= 0 && row < app->lib->nb_cats)
            catdesc_open(app, stat_at(app->lib, row));
    }
    return 1;
}

/*
** the box of a category: counted in TOTAL or not
*/
static int total_box(app_t *app, int stat, float const *rect)
{
    int *total = &app->lib->cats[stat].total;
    float box[4] = {rect[0] + 2, rect[1] + 2, 36, 36};

    widget_tip(app, box, *total ? "Comptée dans le total"
        : "Hors du total");
    if (!widget_icon(app, box, ICON_CHECK, *total ? ICON_FILLED : ICON_DIM))
        return 0;
    *total = !*total;
    db_save_categories(app->lib);
    return 1;
}

/*
** the last row: opens the window of a new category
*/
static int new_row(app_t *app, float const *rect)
{
    float name[4] = {rect[0] + 46, rect[1], rect[2] - 46, rect[3]};

    widget_icon(app, (float [4]){rect[0] + 2, rect[1] + 2, 36, 36},
        ICON_PLUS, ICON_OUTLINE);
    if (!widget_button(app, name, "Nouvelle catégorie"))
        return 0;
    catdesc_open(app, CAT_NEW - 1);
    return 3;
}

/*
** returns 1 if this category was chosen, 2 if its box changed, 3 if the
** window of a new one opens
*/
static int list_row(app_t *app, int row, int current)
{
    int stat = row_stat(app, row);
    float rect[4];
    float name[4];
    char label[64];

    row_rect(row, rect);
    if (stat == ROW_NEW)
        return new_row(app, rect);
    if (stat < CAT_TOTAL && total_box(app, stat, rect))
        return 2;
    snprintf(label, sizeof(label), stat == current ? "> %s <" : "%s",
        category_name(app->lib, stat));
    name[0] = rect[0] + 46;
    name[1] = rect[1];
    name[2] = rect[2] - 46;
    name[3] = rect[3];
    if (stat < CAT_TOTAL)
        widget_tip(app, rect, app->lib->cats[stat].desc);
    return widget_button(app, name, label) ? 1 : 0;
}

/*
** while a category is dragged over another row: a line where it will go
*/
static void drop_line(app_t *app)
{
    sfColor line = {255, 200, 60, 255};
    int from = app->catlist.drag - 1;
    int to = row_at(app);
    float rect[4];

    if (app->wg->mode != WG_DRAW || from < 0 || to < 0
        || to >= app->lib->nb_cats || to == from
        || !sfMouse_isButtonPressed(sfMouseLeft))
        return;
    row_rect(to, rect);
    if (to > from)
        rect[1] += rect[3];
    rect[1] -= 2;
    rect[3] = 4;
    widget_rect(app, rect, &line);
}

/*
** returns 1 if the category changed, 2 if one was put in or out of
** TOTAL. A click anywhere else closes it; the boxes keep it open. Called
** first when clicking (it takes the click), last when drawing (it is
** above the screen).
*/
int category_list(app_t *app, int *category)
{
    catlist_t *list = &app->catlist;
    sfColor back = {20, 16, 36, 255};
    int high = 0;
    int result = 0;
    int got = 0;

    if (!list->open || list->edit)
        return 0;
    list->nb = rows_count(app);
    high = list->nb < ROWS_PER_COL ? list->nb : ROWS_PER_COL;
    widget_rect(app, (float [4]){130, CAT_BAR_Y + 60,
        LIST_COL * ((list->nb - 1) / ROWS_PER_COL + 1),
        high * LIST_ROW + 46}, &back);
    for (int row = 0; row < list->nb && result == 0; row++) {
        got = list_row(app, row, *category);
        if (got == 1)
            *category = row_stat(app, row);
        result = got;
    }
    drop_line(app);
    widget_text(app, "Clic droit : modifier    |    Glisser : déplacer",
        (float [3]){146, CAT_BAR_Y + 66 + high * LIST_ROW, 18}, &gray);
    if (app->wg->mode == WG_CLICK) {
        list->open = result == 2;
        app->wg->mode = WG_IDLE;
    }
    return result == 3 ? 0 : result;
}

/*
** the window of a category if it is open, else the list; total : 1 to
** show TOTAL in the list
*/
int category_overlay(app_t *app, int *category, int total)
{
    app->catlist.total = total;
    if (app->catlist.edit)
        return catdesc_view(app);
    return category_list(app, category);
}
