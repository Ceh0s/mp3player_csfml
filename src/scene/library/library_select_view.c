/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** what the selection mode shows in the library: the button that turns
** it on, a box on the left of each song, and the bar that replaces the
** sort bar (check all, none, open the actions)
*/

#include "mp3player.h"

static const sfColor box = {255, 255, 255, 70};
static const sfColor checked = {250, 200, 60, 255};

/*
** leaving the mode unchecks everything
*/
void library_select_toggle(app_t *app)
{
    libscene_t *scene = app->libscene;
    float rect[4] = {540, 20, 210, 56};

    widget_help(app, rect, "Cocher des titres pour agir sur tous à la fois");
    if (!widget_button(app, rect, scene->selecting ? "> Sélection <"
            : "Sélection"))
        return;
    scene->selecting = !scene->selecting;
    scene->sel.open = 0;
    select_all(app, 0);
}

void library_select_boxes(app_t *app)
{
    libscene_t *scene = app->libscene;
    float y = 0;
    int first = 0;
    int last = 0;

    if (!scene->selecting)
        return;
    list_shown(scene, &first, &last);
    for (int i = first; i < last; i++) {
        y = list_row_y(scene, i) + 11;
        widget_rect(app, (float [4]){94, y, 30, 30}, &box);
        if (select_has(&scene->sel, scene->view[i]->id))
            widget_rect(app, (float [4]){100, y + 6, 18, 18}, &checked);
    }
}

void library_select_bar(app_t *app)
{
    selection_t *sel = &app->libscene->sel;
    float actions[4] = {1580, SORT_Y, 250, 52};
    char label[64];

    if (widget_button(app, (float [4]){LIST_X, SORT_Y, 280, 52},
            "Tout l'onglet"))
        select_all(app, 1);
    if (widget_button(app, (float [4]){LIST_X + 292, SORT_Y, 150, 52},
            "Aucun"))
        select_all(app, 0);
    snprintf(label, sizeof(label), "%d sélectionné(s)   (Maj + clic : "
        "plage)", sel->nb);
    widget_text(app, label, (float [3]){LIST_X + 470, SORT_Y + 12, 26},
        &sfWhite);
    if (sel->nb == 0 || !widget_button(app, actions, "Actions..."))
        return;
    sel->open = 1;
    sel->confirm = 0;
    sel->msg[0] = '\0';
}
