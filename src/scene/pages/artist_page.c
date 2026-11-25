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

static void draw_titles(app_t *app, artist_t *artist)
{
    char line[NAME_LEN + 64];
    char time[16];
    song_t *song = NULL;
    int shown = 0;

    widget_text(app, "Ses titres", (float [3]){130, 450, 26}, &sfWhite);
    for (link_t *link = app->lib->links; link && shown < 8;
        link = link->next) {
        song = song_by_id(app->lib, link->song_id);
        if (link->artist_id != artist->id || !song || !song->present)
            continue;
        snprintf(time, sizeof(time), "-");
        if (song->duration_ms > 0)
            format_time(time, sizeof(time), song->duration_ms);
        snprintf(line, sizeof(line), "%s   -   %s   -   %s",
            song_title(song), part_short[song_part(app->lib, link)], time);
        widget_text(app, line, (float [3]){160, 492 + shown * 40, 22},
            &gray);
        shown++;
    }
}

/*
** *back is set to 1 when the "Retour" button is clicked
*/
void artist_page(app_t *app, artist_t *artist, int *back)
{
    artist_stats_t stats;

    widget_text(app, artist ? artist->name : "Nouvel artiste",
        (float [3]){130, 125, 40}, &sfWhite);
    if (artist) {
        artist_stats(app->lib, artist->id, &stats);
        draw_parts(app, &stats);
        draw_titles(app, artist);
    } else
        widget_text(app, "Pas encore de statistiques.",
            (float [3]){130, 225, 30}, &sfWhite);
    if (artist)
        like_buttons(app, KIND_ARTIST, artist->id,
            (float [3]){420, 846 + app->extra_h, 0});
    if (widget_button(app, (float [4]){130, 840 + app->extra_h, 260, 56},
            "Retour"))
        *back = 1;
}
