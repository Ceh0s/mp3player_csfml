/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** "Départage" tab: the order used between the songs (or artists) with
** the same Elo, % and number of victories; moved by drag and drop
** (ranking_drag.c) or set by title, by artist or at random
*/

#include "mp3player.h"
#include <strings.h>

static const sfColor gray = {180, 180, 180, 255};

static int by_title(void const *a, void const *b)
{
    return strcasecmp(rank_name(a), rank_name(b));
}

/*
** artists of a song (its name for an artist), then the title
*/
static int by_artist(void const *a, void const *b)
{
    rank_t const *first = a;
    rank_t const *second = b;
    char const *one = first->song ? first->song->artists : NULL;
    char const *two = second->song ? second->song->artists : NULL;
    int diff = strcasecmp(one ? one : rank_name(first),
        two ? two : rank_name(second));

    return diff != 0 ? diff : by_title(a, b);
}

static void shuffle(rank_t *order, int nb)
{
    rank_t swap;
    int other = 0;

    for (int i = nb - 1; i > 0; i--) {
        other = rand() % (i + 1);
        swap = order[i];
        order[i] = order[other];
        order[other] = swap;
    }
}

static void buttons(app_t *app, ranking_t *ranking)
{
    float y = 840 + app->extra_h;
    int sorted = 0;

    if (widget_button(app, (float [4]){430, y, 240, 56}, "Par titre")) {
        qsort(ranking->order, ranking->nb, sizeof(rank_t), &by_title);
        sorted = 1;
    }
    if (widget_button(app, (float [4]){690, y, 260, 56}, "Par artiste")) {
        qsort(ranking->order, ranking->nb, sizeof(rank_t), &by_artist);
        sorted = 1;
    }
    if (widget_button(app, (float [4]){970, y, 240, 56}, "Aléatoire")) {
        shuffle(ranking->order, ranking->nb);
        sorted = 1;
    }
    if (sorted)
        ranking_save_order(app);
}

static void tie_line(app_t *app, rank_t const *rank, int i, float y)
{
    char text[16];

    snprintf(text, sizeof(text), "%d", i + 1);
    widget_text(app, text, (float [3]){130, y, 26}, &gray);
    ranking_hover(app, rank, (float [4]){130, y - 8, 1650, BOARD_STEP - 4});
    widget_image(app, image_get(app, rank, 1),
        (float [4]){190, y - 8, THUMB, THUMB}, rank->artist != NULL);
    widget_rank(app, rank, (float [4]){252, y - 8, 1400, BOARD_STEP - 4},
        &sfWhite);
}

void ranking_ties(app_t *app)
{
    ranking_t *ranking = app->ranking;
    int rows = ranking_rows(app);
    int shown = -1;
    char const *help = "Ordre entre ex æquo (même Elo, même % et même "
        "nombre de victoires) : glisse une ligne pour la déplacer.";

    if (app->search.text[0] != '\0')
        help = "Recherche en cours : vide-la pour déplacer les lignes.";
    widget_text(app, help, (float [3]){130, CONTENT_Y, 22}, &gray);
    for (int i = 0; i < ranking->nb; i++) {
        if (!ranking_match(app, &ranking->order[i]))
            continue;
        shown++;
        if (shown >= ranking->scroll && shown < ranking->scroll + rows)
            tie_line(app, &ranking->order[i], i, CONTENT_Y + BOARD_STEP
                + (shown - ranking->scroll) * BOARD_STEP);
    }
    if (app->wg->mode == WG_DRAW)
        ranking_drag_draw(app);
    buttons(app, ranking);
}
