/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** page of an artist: total duration of his songs and its detail
** (drawn with the widgets, so any scene can show it)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static char const *const part_names[NB_PARTS] = {"en original",
    "remixé(s) par d'autres", "en tant que remixeur",
    "en featuring"};

static char const *const part_short[NB_PARTS] = {"original",
    "auteur remixé", "remixeur", "feat."};

static char const *titles_word(int nb)
{
    return nb > 1 ? "titres" : "titre";
}

static void draw_parts(app_t *app, artist_stats_t const *stats)
{
    char line[160];
    char time[16];

    format_time(time, sizeof(time), stats->total_ms);
    snprintf(line, sizeof(line), "%s de musique sur %d %s", time,
        stats->total_titles, titles_word(stats->total_titles));
    widget_text(app, line, (float [3]){130, 225, 30}, &sfWhite);
    for (int i = 0; i < NB_PARTS; i++) {
        format_time(time, sizeof(time), stats->ms[i]);
        snprintf(line, sizeof(line), "dont %s sur %d %s %s", time,
            stats->titles[i], titles_word(stats->titles[i]), part_names[i]);
        widget_text(app, line, (float [3]){160, 280 + i * 40, 24}, &sfWhite);
    }
}

/*
** the present songs of the artist
*/
static song_t *titled_song(app_t *app, artist_t *artist, link_t *link)
{
    song_t *song = song_by_id(app->lib, link->song_id);

    if (link->artist_id != artist->id || !song || !song->present)
        return NULL;
    return song;
}

static void title_line(app_t *app, song_t *song, link_t *link, float y)
{
    char line[NAME_LEN + 64];
    char time[16];

    snprintf(time, sizeof(time), "-");
    if (song->duration_ms > 0)
        format_time(time, sizeof(time), song->duration_ms);
    snprintf(line, sizeof(line), "%s   -   %s   -   %s",
        song_title(song), part_short[song_part(app->lib, link)], time);
    widget_fit(app, line, (float [4]){160, y, 22, 1100}, &gray);
}

/*
** its songs, left of the image, from y 488 to the bottom; they scroll
*/
static void draw_titles(app_t *app, artist_t *artist, scroll_t *titles)
{
    float area[4] = {120, 488, 1140, 820 + app->extra_h - 488};
    song_t *song = NULL;
    int nb = 0;
    float y = 0;

    widget_text(app, "Ses titres", (float [3]){130, 450, 26}, &sfWhite);
    for (link_t *link = app->lib->links; link; link = link->next)
        nb += titled_song(app, artist, link) != NULL;
    scroll_frame(titles, area, nb * 40);
    scroll_bar_draw(app, titles);
    if (!scroll_rows_begin(app, titles))
        return;
    nb = 0;
    for (link_t *link = app->lib->links; link; link = link->next) {
        song = titled_song(app, artist, link);
        if (!song)
            continue;
        y = scroll_y(titles, nb * 40 + 4);
        nb++;
        if (scroll_seen(titles, y, 40))
            title_line(app, song, link, y);
    }
    scroll_rows_end(app);
}

/*
** *back is set to 1 when the "Retour" button is clicked
*/
void artist_page(app_t *app, artist_t *artist, scroll_t *titles,
    int *back)
{
    artist_stats_t stats;

    widget_text(app, artist ? artist->name : "Nouvel artiste",
        (float [3]){130, 125, 40}, &sfWhite);
    if (artist) {
        artist_stats(app->lib, artist->id, &stats);
        draw_parts(app, &stats);
        draw_titles(app, artist, titles);
    } else
        widget_text(app, "Pas encore de statistiques.",
            (float [3]){130, 225, 30}, &sfWhite);
    if (artist)
        like_buttons(app, KIND_ARTIST, artist->id,
            (float [3]){130, 846 + app->extra_h, 0});
    if (widget_back(app))
        *back = 1;
}
