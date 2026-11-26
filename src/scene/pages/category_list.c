/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** list of the categories opened by the category bar (category_bar.c):
** a click chooses one, its box counts it in TOTAL or not, the mouse
** over it shows its description, a right click opens the window where
** the description is typed (also opened from the stats of a page)
*/

#include "mp3player.h"

static const sfColor gray = {190, 190, 190, 255};

    #define LIST_ROW 44

static void row_rect(int row, float *rect)
{
    rect[0] = 136;
    rect[1] = CAT_BAR_Y + 66 + row * LIST_ROW;
    rect[2] = 478;
    rect[3] = LIST_ROW - 4;
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

void catdesc_open(app_t *app, int stat)
{
    catlist_t *list = &app->catlist;

    list->edit = stat + 1;
    list->flaw = app->lib->cats[stat].flaw;
    snprintf(list->text, DESC_LEN, "%s", app->lib->cats[stat].desc);
}

static void catdesc_save(app_t *app)
{
    catlist_t *list = &app->catlist;
    int stat = list->edit - 1;

    category_set_desc(app->lib, stat, list->text);
    app->lib->cats[stat].flaw = list->flaw;
    db_save_categories(app->lib);
    list->edit = 0;
}

/*
** while the list or the window is open, it takes the keys, the wheel and
** the right clicks (returns 1); the left clicks go through the view of
** the scene, which gives them to the list first
*/
int catlist_event(app_t *app, sfEvent *event)
{
    catlist_t *list = &app->catlist;
    int row = 0;

    if (!list->open && !list->edit)
        return 0;
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
        else
            text_input_n(app, list->text, DESC_LEN, event->text.unicode);
    }
    if (event->type == sfEvtMouseButtonPressed && !list->edit
        && event->mouseButton.button == sfMouseRight) {
        row = row_at(app);
        if (row >= 0 && row < NB_CATEGORIES)
            catdesc_open(app, row);
    }
    return 1;
}

/*
** the box of a category: counted in TOTAL or not
*/
static int total_box(app_t *app, int row, float const *rect)
{
    int *total = &app->lib->cats[row].total;
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
** returns 1 if this category was chosen, 2 if its box changed
*/
static int list_row(app_t *app, int row, int current)
{
    float rect[4];
    float name[4];
    char label[64];

    row_rect(row, rect);
    if (row < NB_CATEGORIES && total_box(app, row, rect))
        return 2;
    snprintf(label, sizeof(label), row == current ? "> %s <" : "%s",
        category_name(row));
    name[0] = rect[0] + 46;
    name[1] = rect[1];
    name[2] = rect[2] - 46;
    name[3] = rect[3];
    if (row < NB_CATEGORIES)
        widget_tip(app, rect, app->lib->cats[row].desc);
    return widget_button(app, name, label) ? 1 : 0;
}

/*
** nb : rows (NB_CATEGORIES + 1 with TOTAL); returns 1 if the category
** changed, 2 if one was put in or out of TOTAL. A click anywhere else
** closes it; the boxes keep it open. Called first when clicking (it takes
** the click), last when drawing (it is above the screen).
*/
int category_list(app_t *app, int *category, int nb)
{
    catlist_t *list = &app->catlist;
    sfColor back = {20, 16, 36, 255};
    int result = 0;
    int got = 0;

    if (!list->open || list->edit)
        return 0;
    list->nb = nb;
    widget_rect(app, (float [4]){130, CAT_BAR_Y + 60, 490,
        nb * LIST_ROW + 46}, &back);
    for (int row = 0; row < nb && result == 0; row++) {
        got = list_row(app, row, *category);
        if (got == 1)
            *category = row;
        result = got;
    }
    widget_text(app, "Clic droit : description",
        (float [3]){146, CAT_BAR_Y + 66 + nb * LIST_ROW, 18}, &gray);
    if (app->wg->mode == WG_CLICK) {
        list->open = result == 2;
        app->wg->mode = WG_IDLE;
    }
    return result;
}

static void measure_button(app_t *app, float x, float y)
{
    float rect[4] = {x, y, 380, 52};
    catlist_t *list = &app->catlist;

    if (widget_button(app, rect, list->flaw ? "Mesure : un défaut"
            : "Mesure : une qualité"))
        list->flaw = !list->flaw;
}

/*
** the window of the description, over the screen; returns 1 once saved
** (a flaw counts the other way in TOTAL). Called like category_list.
*/
int catdesc_view(app_t *app)
{
    catlist_t *list = &app->catlist;
    sfFloatRect view = app->view_rect;
    float win[4] = {560, 300, 800, 330};
    sfColor shade = {0, 0, 0, 170};
    sfColor back = {30, 30, 30, 245};
    int saved = 0;

    if (!list->edit)
        return 0;
    widget_rect(app, (float [4]){view.left, view.top, view.width,
        view.height}, &shade);
    widget_rect(app, win, &back);
    widget_text(app, stat_name(list->edit - 1),
        (float [3]){590, 320, 32}, &sfWhite);
    widget_field(app, (float [4]){590, 380, 740, 56}, list->text, 1);
    measure_button(app, 590, 456);
    if (widget_icon_button(app, (float [4]){590, 540, 260, 56}, ICON_SAVE,
            "Enregistrer")) {
        catdesc_save(app);
        saved = 1;
    }
    if (widget_icon_tip(app, (float [4]){1372, 300, 48, 48}, ICON_CLOSE,
            "Fermer"))
        list->edit = 0;
    if (app->wg->mode == WG_CLICK)
        app->wg->mode = WG_IDLE;
    return saved;
}

/*
** the window of the description if it is open, else the list
*/
int category_overlay(app_t *app, int *category, int nb)
{
    if (app->catlist.edit)
        return catdesc_view(app) ? 2 : 0;
    return category_list(app, category, nb);
}
