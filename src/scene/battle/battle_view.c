/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the battle screen: ranking, category, the two cards,
** buttons (called to draw it and to handle a click, see widgets.c)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static void card_stats(app_t *app, rank_t const *rank, float x)
{
    rank_t score = *rank;
    char line[160];
    int percent = 0;

    elo_score(app->lib, &app->arena, &score);
    if (score.battles > 0)
        percent = score.wins * 100 / score.battles;
    snprintf(line, sizeof(line), "Elo %.0f en %s   -   %d battle%s   -   "
        "%d %%", score.rating, category_name(app->arena.category),
        score.battles, score.battles > 1 ? "s" : "", percent);
    widget_text(app, line, (float [3]){x, BATTLE_CARD_Y + 356, 22}, &gray);
}

/*
** artists of a song, or listening time of an artist
*/
static void card_detail(app_t *app, rank_t const *rank, float x)
{
    char line[64];
    char time[16];
    long ms = 0;

    if (rank->song) {
        widget_text(app, rank->song->artists ? rank->song->artists
            : "Artiste inconnu", (float [3]){x, BATTLE_CARD_Y + 318, 24},
            &gray);
        return;
    }
    ms = artist_listen_ms(app->lib, rank->artist->id);
    format_time(time, sizeof(time), ms);
    snprintf(line, sizeof(line), "%s d'écoute", time);
    widget_text(app, line, (float [3]){x, BATTLE_CARD_Y + 318, 24}, &gray);
}

/*
** the whole card is a button: a click on it is a vote (but not on its
** "Écouter" or its progress bar, tested first); the big image is
** centered at its top (round for an artist)
*/
static int card(app_t *app, rank_t const *rank, float x)
{
    float picture[4] = {x + 255, BATTLE_CARD_Y + 16, 250, 250};
    float name[4] = {x + 30, BATTLE_CARD_Y + 276, 32, 700};
    int clicked = 0;

    if (app->wg->mode == WG_CLICK && battle_listen(app, rank, x))
        return 0;
    clicked = widget_button(app,
        (float [4]){x, BATTLE_CARD_Y, 760, BATTLE_CARD_H}, "");
    widget_image(app, image_get(app, rank, 0), picture, rank->artist != NULL);
    widget_fit(app, rank_name(rank), name, &sfWhite);
    card_detail(app, rank, x + 30);
    card_stats(app, rank, x + 30);
    if (app->wg->mode == WG_DRAW)
        battle_listen(app, rank, x);
    return clicked;
}

static void cards(app_t *app, battle_t *battle)
{
    float middle = BATTLE_CARD_Y + BATTLE_CARD_H / 2 - 30;

    widget_text(app, "VS", (float [3]){930, middle, 48}, &sfWhite);
    if (card(app, &battle->pair[0], 130)) {
        battle_vote(app, 0);
        return;
    }
    if (card(app, &battle->pair[1], 1030)) {
        battle_vote(app, 1);
        return;
    }
    if (battle->focus >= 0)
        battle_frame(app, battle->focus == 0 ? 130 : 1030);
}

/*
** the category in play, centered under the cards: its name, then its
** description (useful when it was drawn by "Catégorie aléatoire")
*/
static void category_shown(app_t *app)
{
    int category = app->arena.category;
    char const *name = category_name(category);
    char const *desc = category < NB_CATEGORIES
        ? app->lib->cats[category].desc : "Somme des catégories cochées.";
    float y = BATTLE_CARD_Y + BATTLE_CARD_H + 10;
    float width = widget_text_width(app, name, 34);

    widget_text(app, name, (float [3]){960 - width / 2, y, 34}, &sfWhite);
    width = widget_text_width(app, desc, 22);
    if (width > 1650)
        width = 1650;
    widget_fit(app, desc, (float [4]){960 - width / 2, y + 48, 22, 1650},
        &gray);
}

static void bottom(app_t *app, battle_t *battle)
{
    float y = 840 + app->extra_h;
    float skip[4] = {1530, y, 260, 56};
    float match[4] = {1090, y, 420, 56};
    float history[4] = {130, y, 260, 56};
    int *on = &app->lib->settings.battle_match;

    if (widget_back(app))
        scene_back(app);
    if (battle->has_pair && app->arena.kind == KIND_SONG)
        widget_text(app, "Clic droit : page de la musique",
            (float [3]){410, y + 16, 22}, &gray);
    if (widget_icon_button(app, history, ICON_CLOCK, "Historique")) {
        battle_open_history(app);
        return;
    }
    widget_help(app, match, "Équilibrées : peu de battles, Elo proche");
    if (widget_button(app, match, *on ? "Rencontres : équilibrées"
            : "Rencontres : au hasard")) {
        *on = !*on;
        db_save_settings(app->lib);
        battle_next(app);
        return;
    }
    if (battle->has_pair && widget_icon_button(app, skip, ICON_SKIP,
            "Passer"))
        battle_next(app);
}

/*
** the list of the categories is above the screen: it takes the clicks
** first and is drawn last
*/
void battle_view(app_t *app)
{
    battle_t *battle = app->battle;
    int *category = &app->arena.category;

    if (app->wg->mode == WG_CLICK
        && category_overlay(app, category, NB_CATEGORIES) == 1) {
        app->lib->settings.battle_random = 0;
        db_save_settings(app->lib);
        battle_next(app);
        return;
    }
    widget_text(app, "Battle", (float [3]){130, 115, 40}, &sfWhite);
    if (board_bar(app, &app->arena))
        battle_reload(app);
    if (category_bar(app, &app->arena.category, 1))
        battle_next(app);
    if (battle->has_pair) {
        category_shown(app);
        cards(app, battle);
    } else
        widget_text(app, app->arena.kind == KIND_SONG ? "Pas assez de "
            "musiques pour une battle." : "Pas assez d'artistes pour une "
            "battle.",
            (float [3]){130, CONTENT_Y, 30}, &sfWhite);
    bottom(app, battle);
    if (app->wg->mode == WG_DRAW)
        category_overlay(app, category, NB_CATEGORIES);
}
