/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** buttons of the bottom bar of the library choosing how the queue is
** played: random order, and repeat (off / the queue / the song), kept
** in settings.tsv
*/

#include "mp3player.h"

void library_play_modes(app_t *app)
{
    char const *repeats[3] = {"Boucle : non", "Boucle : file",
        "Boucle : titre"};
    float shuffle[4] = {1470, 990 + app->extra_h, 220, 50};
    float repeat[4] = {1700, 990 + app->extra_h, 200, 50};
    settings_t const *settings = &app->lib->settings;

    widget_help(app, shuffle, "Joue la file dans un ordre au hasard (le "
        "titre en cours reste)");
    widget_help(app, repeat, "Non, toute la file à la suite, ou le même "
        "titre en boucle");
    if (widget_button(app, shuffle, settings->shuffle ? "Aléatoire : oui"
            : "Aléatoire : non"))
        player_shuffle_toggle(app);
    if (widget_button(app, repeat, repeats[settings->repeat]))
        player_repeat_next(app);
}
