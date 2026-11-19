/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** content of the battle screen: category, the two cards, buttons
** (called to draw it and to handle a click, see widgets.c)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static void card_stats(app_t *app, song_t *song, int category, float x)
{
    rank_t score;
    char line[96];
    int percent = 0;

    elo_score(app->lib, song, category, &score);
    if (score.battles > 0)
        percent = score.wins * 100 / score.battles;
    snprintf(line, sizeof(line), "Elo %.0f   -   %d battle%s   -   %d %% "
        "de victoires", score.rating, score.battles,
        score.battles > 1 ? "s" : "", percent);
    widget_text(app, line, (float [3]){x + 30, 640, 24}, &gray);
}

/*
** the whole card is a button: a click on it is a vote
*/
static int card(app_t *app, song_t *song, int category, float x)
{
    int clicked = widget_button(app, (float [4]){x, 300, 760, 420}, "");

    widget_text(app, song_title(song), (float [3]){x + 30, 400, 36},
        &sfWhite);
    widget_text(app, song->artists ? song->artists : "Artiste inconnu",
        (float [3]){x + 30, 470, 28}, &gray);
    card_stats(app, song, category, x);
    return clicked;
}

static void cards(app_t *app, battle_t *battle)
{
    widget_text(app, "VS", (float [3]){930, 480, 48}, &sfWhite);
    widget_text(app, "Clique sur ta préférée (ou flèches gauche / droite)",
        (float [3]){130, 745, 24}, &gray);
    if (card(app, battle->pair[0], battle->category, 130)) {
        battle_vote(app, 0);
        return;
    }
    if (card(app, battle->pair[1], battle->category, 1030))
        battle_vote(app, 1);
}

void battle_view(app_t *app)
{
    battle_t *battle = app->battle;
    float y = 840 + app->extra_h;

    widget_text(app, "Battle", (float [3]){130, 125, 40}, &sfWhite);
    if (category_bar(app, &battle->category, NB_CATEGORIES, 1))
        battle_next(app);
    if (battle->has_pair)
        cards(app, battle);
    else
        widget_text(app, "Il faut au moins deux musiques pour une battle.",
            (float [3]){130, 300, 30}, &sfWhite);
    if (widget_button(app, (float [4]){130, y, 260, 56}, "Retour"))
        scene_back(app);
    if (battle->has_pair
        && widget_button(app, (float [4]){1530, y, 260, 56}, "Passer"))
        battle_next(app);
}
