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
    char line[96];
    int percent = 0;

    elo_score(app->lib, &app->arena, &score);
    if (score.battles > 0)
        percent = score.wins * 100 / score.battles;
    snprintf(line, sizeof(line), "Elo %.0f   -   %d battle%s   -   %d %% "
        "de victoires", score.rating, score.battles,
        score.battles > 1 ? "s" : "", percent);
    widget_text(app, line, (float [3]){x, BATTLE_CARD_Y + 404, 22}, &gray);
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
            : "Artiste inconnu", (float [3]){x, BATTLE_CARD_Y + 364, 24},
            &gray);
        return;
    }
    ms = artist_listen_ms(app->lib, rank->artist->id);
    format_time(time, sizeof(time), ms);
    snprintf(line, sizeof(line), "%s d'écoute", time);
    widget_text(app, line, (float [3]){x, BATTLE_CARD_Y + 364, 24}, &gray);
}

/*
** the whole card is a button: a click on it is a vote (but not on its
** "Écouter" or its progress bar, tested first); the big image is
** centered at its top (round for an artist)
*/
static int card(app_t *app, rank_t const *rank, float x)
{
    float picture[4] = {x + 235, BATTLE_CARD_Y + 16, 290, 290};
    int clicked = 0;

    if (app->wg->mode == WG_CLICK && battle_listen(app, rank, x))
        return 0;
    clicked = widget_button(app,
        (float [4]){x, BATTLE_CARD_Y, 760, BATTLE_CARD_H}, "");
    widget_image(app, image_get(app, rank, 0), picture, rank->artist != NULL);
    widget_text(app, rank_name(rank),
        (float [3]){x + 30, BATTLE_CARD_Y + 320, 32}, &sfWhite);
    card_detail(app, rank, x + 30);
    card_stats(app, rank, x + 30);
    if (app->wg->mode == WG_DRAW)
        battle_listen(app, rank, x);
    return clicked;
}

static void cards(app_t *app, battle_t *battle)
{
    float y = BATTLE_CARD_Y + BATTLE_CARD_H;
    float middle = BATTLE_CARD_Y + BATTLE_CARD_H / 2 - 30;

    widget_text(app, "VS", (float [3]){930, middle, 48}, &sfWhite);
    widget_text(app, "Clique sur ton choix, ou flèches gauche / droite "
        "puis Entrée", (float [3]){130, y + 6, 22}, &gray);
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

static void bottom(app_t *app, battle_t *battle)
{
    float y = 840 + app->extra_h;
    float skip[4] = {1530, y, 260, 56};

    if (widget_button(app, (float [4]){130, y, 260, 56}, "Retour"))
        scene_back(app);
    widget_help(app, skip, "Autre paire, sans vote");
    if (battle->has_pair && widget_button(app, skip, "Passer"))
        battle_next(app);
}

void battle_view(app_t *app)
{
    battle_t *battle = app->battle;

    widget_text(app, "Battle", (float [3]){130, 115, 40}, &sfWhite);
    if (board_bar(app, &app->arena))
        battle_reload(app);
    if (category_bar(app, &app->arena.category, NB_CATEGORIES, 1))
        battle_next(app);
    if (battle->has_pair)
        cards(app, battle);
    else
        widget_text(app, app->arena.kind == KIND_SONG ? "Il faut au moins "
            "deux musiques dans ce classement pour une battle." : "Il faut "
            "au moins deux artistes dans ce classement pour une battle.",
            (float [3]){130, CONTENT_Y, 30}, &sfWhite);
    bottom(app, battle);
}
