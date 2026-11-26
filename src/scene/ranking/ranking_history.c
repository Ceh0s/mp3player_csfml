/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** "Historique" tab of the rankings screen: the battles of the ranking
** and kind shown (elo_history.c), the last one first, of the category
** chosen (TOTAL = every category, then written on the row). Each side
** shows its Elo and its place before and after, green for a gain, red
** for a loss, gray if nothing changed; on the right, the record of the
** two against each other. Right click on a side = its page. The last
** battle of the ranking can be undone (elo_undo.c), then the one before,
** UNDO_MAX in a row at most.
*/

#include "mp3player.h"
#include <math.h>
#include <time.h>

static const sfColor gray = {180, 180, 180, 255};
static const sfColor green = {90, 200, 90, 255};
static const sfColor red = {230, 80, 80, 255};

static sfColor const *change_color(long change)
{
    if (change > 0)
        return &green;
    return change < 0 ? &red : &gray;
}

static rank_t subject(app_t *app, int id)
{
    rank_t rank = {NULL, NULL, 0, 0, 0};

    if (app->arena.kind == KIND_SONG)
        rank.song = song_by_id(app->lib, id);
    else
        rank.artist = artist_by_id(app->lib, id);
    return rank;
}

/*
** of the category chosen, and matching the search (a reset is only
** shown without a search)
*/
static int shown(app_t *app, hist_t const *item)
{
    rank_t sides[2];

    if (item->reset)
        return app->search.text[0] == '\0';
    if (app->arena.category != CAT_TOTAL
        && item->category != app->arena.category)
        return 0;
    sides[0] = subject(app, item->ids[0]);
    sides[1] = subject(app, item->ids[1]);
    return (sides[0].song || sides[0].artist || sides[1].song
        || sides[1].artist) && (ranking_match(app, &sides[0])
        || ranking_match(app, &sides[1]));
}

/*
** built again after a change of ranking, kind or a reset
*/
static void build(app_t *app)
{
    ranking_t *ranking = app->ranking;
    history_t *history = &ranking->history;

    if (!ranking->history_dirty)
        return;
    history_free(history);
    elo_history(app->lib, &app->arena, history);
    ranking->history_dirty = 0;
    ranking->undo_left = 0;
    if (history->nb > 0 && !history->items[history->nb - 1].reset)
        ranking->undo_left = battle_undo_left(&app->arena,
            &history->items[history->nb - 1]);
}

int ranking_history_count(app_t *app)
{
    history_t const *history = &app->ranking->history;
    int nb = 0;

    build(app);
    for (int i = 0; i < history->nb; i++)
        nb += shown(app, &history->items[i]);
    return nb;
}

static void when(app_t *app, hist_t const *item, float y)
{
    time_t date = item->date;
    struct tm *tm = localtime(&date);
    char text[32];

    strftime(text, sizeof(text), "%d/%m/%Y", tm);
    widget_text(app, text, (float [3]){130, y + 4, 22}, &gray);
    strftime(text, sizeof(text), "%H:%M", tm);
    widget_text(app, text, (float [3]){130, y + 34, 22}, &gray);
    if (item->reset || app->arena.category != CAT_TOTAL)
        return;
    utf8_cut(text, elo_categories[item->category], 14);
    widget_text(app, text, (float [3]){130, y + 64, 18}, &gray);
}

/*
** Elo and place before and after of the side (0 winner, 1 loser)
*/
static void changes(app_t *app, hist_t const *item, int side, float x,
    float y)
{
    long elo = lround(item->after[side]) - lround(item->before[side]);
    int places = item->place_before[side] - item->place_after[side];
    char text[64];

    snprintf(text, sizeof(text), "Elo %.0f → %.0f (%+ld)",
        item->before[side], item->after[side], elo);
    widget_text(app, text, (float [3]){x, y, 20}, change_color(elo));
    if (places == 0)
        snprintf(text, sizeof(text), "#%d → #%d (=)",
            item->place_before[side], item->place_after[side]);
    else
        snprintf(text, sizeof(text), "#%d → #%d (%+d)",
            item->place_before[side], item->place_after[side], places);
    widget_text(app, text, (float [3]){x + 290, y, 20},
        change_color(places));
}

static void side(app_t *app, hist_t const *item, int which, float y)
{
    float x = which == 0 ? 300 : 980;
    rank_t rank = subject(app, item->ids[which]);
    float block[4] = {x, y, 620, HISTORY_STEP - 6};

    if (!rank.song && !rank.artist)
        return;
    ranking_hover(app, &rank, block);
    widget_image(app, image_get(app, &rank, 1),
        (float [4]){x, y + 2, THUMB, THUMB}, rank.artist != NULL);
    widget_rank(app, &rank, (float [4]){x + 66, y, 550, 52}, &sfWhite);
    changes(app, item, which, x + 66, y + 62);
}

static void draw_item(app_t *app, hist_t const *item, float y)
{
    char text[32];

    when(app, item, y);
    if (item->reset) {
        widget_text(app, "Remise à zéro des Elo de ce classement",
            (float [3]){300, y + 20, 24}, &gray);
        return;
    }
    side(app, item, 0, y);
    widget_text(app, "bat", (float [3]){932, y + 14, 22}, &gray);
    side(app, item, 1, y);
    snprintf(text, sizeof(text), "%d – %d", item->wins[0], item->wins[1]);
    widget_text(app, text, (float [3]){1640, y + 4, 28}, &sfWhite);
    widget_text(app, "face à face", (float [3]){1640, y + 44, 18}, &gray);
}

/*
** on the last battle of the ranking, asked twice; returns 1 if it was
** undone (the history is then built again: item is freed)
*/
static int undo_button(app_t *app, hist_t const *item, float y)
{
    ranking_t *ranking = app->ranking;
    history_t const *history = &ranking->history;
    float rect[4] = {1626, y + 62, 154, 36};
    char label[64];

    if (ranking->undo_left == 0 || item != &history->items[history->nb - 1])
        return 0;
    snprintf(label, sizeof(label), ranking->undo_ask ? "Confirmer ?"
        : "Annuler (%d)", ranking->undo_left);
    widget_help(app, rect, "3 de suite au plus");
    if (!widget_button(app, rect, label))
        return 0;
    ranking->undo_ask = !ranking->undo_ask;
    if (ranking->undo_ask)
        return 0;
    battle_undo(app->lib, &app->arena, item);
    ranking_build(app);
    return 1;
}

/*
** the last battle first
*/
void ranking_history(app_t *app)
{
    history_t const *history = &app->ranking->history;
    scroll_t *list = &app->ranking->list;
    int count = ranking_history_count(app);
    int row = 0;
    float y = 0;

    if (count == 0)
        widget_text(app, history->nb == 0 ? "Aucune battle dans ce "
            "classement." : "Aucune battle trouvée.",
            (float [3]){130, CONTENT_Y, 30}, &sfWhite);
    ranking_list_frame(app, CONTENT_Y, count * HISTORY_STEP);
    if (!scroll_rows_begin(app, list))
        return;
    for (int i = history->nb - 1; i >= 0; i--) {
        if (!shown(app, &history->items[i]))
            continue;
        y = scroll_y(list, row * HISTORY_STEP);
        row++;
        if (!scroll_seen(list, y, HISTORY_STEP))
            continue;
        draw_item(app, &history->items[i], y);
        if (undo_button(app, &history->items[i], y))
            break;
    }
    scroll_rows_end(app);
}
