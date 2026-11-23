/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** rows of the statistics screen, built again when a choice changes:
** the songs of the playlist chosen (or every present song) with their
** listening time (all of it, or between the two dates), or the artists
** of these songs (the sum of their songs), the Elo if asked, sorted
*/

#include "mp3player.h"

/*
** by_id gets a row per song of the view (nb songs), with its numbers;
** a wrong date counts as no limit
*/
static void fill_songs(app_t *app, stat_row_t *by_id, song_t **view,
    int nb)
{
    stats_opt_t const *opt = &app->stats_opt;
    long range[2] = {stat_date(opt->from, 0), stat_date(opt->to, 1)};

    for (int i = 0; i < nb; i++)
        by_id[view[i]->id].song = view[i];
    if (range[0] > 0 || range[1] > 0) {
        stat_period(app->lib, app->stats, range, by_id);
        return;
    }
    for (int i = 0; i < nb; i++)
        stat_totals(app->lib, &by_id[view[i]->id]);
}

/*
** the rows of the artists found, in the order of their ids
*/
static int compact(stat_row_t const *art, int size, stat_row_t *rows)
{
    int nb = 0;

    for (int i = 0; i < size; i++)
        if (art[i].artist) {
            rows[nb] = art[i];
            nb++;
        }
    return nb;
}

/*
** an artist gets the numbers of every song it is linked to (any role)
*/
static int artist_rows(library_t *lib, stat_row_t const *by_id,
    stat_row_t *rows)
{
    stat_row_t *art = calloc(lib->next_artist + 1, sizeof(stat_row_t));
    artist_t *artist = NULL;
    int nb = 0;

    if (!art)
        return 0;
    for (link_t *link = lib->links; link; link = link->next) {
        artist = artist_by_id(lib, link->artist_id);
        if (link->song_id >= lib->next_song || !by_id[link->song_id].song
            || !artist || artist->id > lib->next_artist)
            continue;
        art[artist->id].artist = artist;
        art[artist->id].ms += by_id[link->song_id].ms;
        art[artist->id].plays += by_id[link->song_id].plays;
        art[artist->id].skips += by_id[link->song_id].skips;
    }
    nb = compact(art, lib->next_artist + 1, rows);
    free(art);
    return nb;
}

/*
** the Elo of the general ranking in the category chosen
*/
static void add_elo(app_t *app, stat_row_t *rows, int nb)
{
    int category = app->stats_opt.elo - 1;
    arena_t arena = {KIND_SONG, BOARD_GENERAL, category};
    rank_t rank;

    for (int i = 0; category >= 0 && i < nb; i++) {
        memset(&rank, 0, sizeof(rank));
        rank.song = rows[i].song;
        rank.artist = rows[i].artist;
        arena.kind = rows[i].artist ? KIND_ARTIST : KIND_SONG;
        elo_score(app->lib, &arena, &rank);
        rows[i].elo = rank.rating;
    }
}

/*
** the rows of the view (the songs or their artists); returns their
** number, -1 if the memory is missing
*/
static int make_rows(app_t *app, stat_row_t *by_id, song_t **view)
{
    library_t *lib = app->lib;
    playlist_t *pl = playlist_by_id(lib, app->stats_opt.playlist_id);
    int nb = playlist_sorted_view(lib, pl, view);
    int size = nb > lib->next_artist ? nb : lib->next_artist + 1;

    app->stats->rows = malloc(sizeof(stat_row_t) * (size + 1));
    if (!app->stats->rows)
        return -1;
    fill_songs(app, by_id, view, nb);
    if (app->stats_opt.artists)
        return artist_rows(lib, by_id, app->stats->rows);
    for (int i = 0; i < nb; i++)
        app->stats->rows[i] = by_id[view[i]->id];
    return nb;
}

void stats_build(app_t *app)
{
    statscene_t *scene = app->stats;
    stats_opt_t const *opt = &app->stats_opt;
    stat_row_t *by_id = calloc(app->lib->next_song + 1, sizeof(stat_row_t));
    song_t **view = malloc(sizeof(song_t *) * (app->lib->nb + 1));
    int sort = opt->sort == STAT_SORT_ELO && !opt->elo ? STAT_SORT_TIME
        : opt->sort;

    free(scene->rows);
    scene->rows = NULL;
    scene->nb = 0;
    scene->dirty = 0;
    if (by_id && view)
        scene->nb = make_rows(app, by_id, view);
    if (scene->nb < 0)
        scene->nb = 0;
    add_elo(app, scene->rows, scene->nb);
    stat_sort(scene->rows, scene->nb, sort, opt->ascending);
    free(by_id);
    free(view);
    stats_scroll(app, 0);
}
