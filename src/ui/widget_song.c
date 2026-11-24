/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** a song as in YT Music: its title, and its artists smaller and gray
** just under it; both end with "…" when they are too long for the
** width given (text_fit.c)
*/

#include "mp3player.h"

static const sfColor gray = {175, 175, 175, 255};

/*
** pos = {x, y, character size, width allowed}
*/
void widget_fit(app_t *app, char const *str, float const *pos,
    sfColor const *color)
{
    sfText *text = app->wg->text;

    if (app->wg->mode != WG_DRAW)
        return;
    sfText_setCharacterSize(text, pos[2]);
    sfText_setFillColor(text, *color);
    text_fit(app, text, str, pos[3]);
    sfText_setPosition(text, (sfVector2f){pos[0], pos[1]});
    sfRenderWindow_drawText(app->window, text, NULL);
}

/*
** rect = {x, y, w, h}: the title takes half of the height, the artists
** a third; color : of the title (the artists stay gray)
*/
void widget_song(app_t *app, song_t const *song, float const *rect,
    sfColor const *color)
{
    float title[4] = {rect[0], rect[1] - 2, rect[3] * 0.5, rect[2]};
    float artists[4] = {rect[0], rect[1] + rect[3] * 0.575,
        rect[3] * 0.34, rect[2]};

    widget_fit(app, song_title(song), title, color);
    widget_fit(app, song->artists ? song->artists : "Artiste inconnu",
        artists, &gray);
}

/*
** a row of a ranking: a song (title and artists), or the name of an
** artist alone, in the middle of the height
*/
void widget_rank(app_t *app, rank_t const *rank, float const *rect,
    sfColor const *color)
{
    float name[4] = {rect[0], rect[1] + rect[3] * 0.18, rect[3] * 0.5,
        rect[2]};

    if (rank->song) {
        widget_song(app, rank->song, rect, color);
        return;
    }
    widget_fit(app, rank_name(rank), name, color);
}
