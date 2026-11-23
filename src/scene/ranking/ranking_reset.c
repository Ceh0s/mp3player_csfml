/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** window over the rankings screen to reset the battle stats of the
** ranking shown or of all of them; its buttons only work once
** "Confirm" is typed, since the Elo cannot be brought back from the app
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor red = {255, 110, 110, 255};

static void texts(app_t *app, board_t const *board)
{
    char line[NAME_LEN + 64];

    widget_text(app, "Réinitialiser les battles",
        (float [3]){500, 300, 36}, &sfWhite);
    widget_text(app, "Les Elo repartent de 1500 et les battles de zéro. "
        "L'historique reste dans battles.log,", (float [3]){500, 360, 22},
        &gray);
    widget_text(app, "mais l'appli ne pourra pas les retrouver.",
        (float [3]){500, 390, 22}, &gray);
    snprintf(line, sizeof(line), "Ce classement : %s, %s (toutes les "
        "catégories)", board ? board->name : "?",
        app->arena.kind == KIND_SONG ? "musiques" : "artistes");
    widget_text(app, line, (float [3]){500, 440, 22}, &sfWhite);
    widget_text(app, "Tape Confirm pour activer les boutons :",
        (float [3]){500, 490, 22}, &sfWhite);
}

static void reset(app_t *app, int all)
{
    elo_reset(app->lib, &app->arena, all);
    app->ranking->reset = 0;
    app->ranking->confirm[0] = '\0';
    ranking_build(app);
}

void ranking_reset_view(app_t *app)
{
    ranking_t *ranking = app->ranking;
    int ok = strcmp(ranking->confirm, "Confirm") == 0;
    sfColor back = {15, 15, 15, 245};
    sfColor dim = {0, 0, 0, 170};

    widget_rect(app, (float [4]){0, 0, 1920, 1080 + app->extra_h}, &dim);
    widget_rect(app, (float [4]){460, 270, 1000, 480}, &back);
    texts(app, board_by_id(app->lib, app->arena.board));
    widget_field(app, (float [4]){500, 530, 400, 56}, ranking->confirm, 1);
    if (!ok)
        widget_text(app, "Boutons inactifs", (float [3]){930, 545, 22}, &red);
    if (widget_button(app, (float [4]){500, 650, 280, 56}, "Ce classement")
        && ok)
        reset(app, 0);
    if (widget_button(app, (float [4]){800, 650, 280, 56}, "Tout") && ok)
        reset(app, 1);
    if (widget_button(app, (float [4]){1100, 650, 320, 56}, "Annuler")) {
        ranking->reset = 0;
        ranking->confirm[0] = '\0';
    }
}
