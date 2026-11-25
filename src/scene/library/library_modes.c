/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** icons of the bottom bar of the library around play / pause / skip:
** stop, back and forward by 10 s, how the queue is played (random
** order, repeat: off / the queue / the song, kept in settings.tsv) and
** the volume
*/

#include "mp3player.h"

static const sfColor gray = {200, 200, 200, 255};

static void moves(app_t *app, float y)
{
    float stop[4] = {640, y, 56, 56};
    float back[4] = {712, y, 56, 56};
    float forward[4] = {1150, y, 56, 56};

    widget_help(app, stop, "Arrêter");
    widget_help(app, back, "-10 s");
    widget_help(app, forward, "+10 s");
    if (widget_icon(app, stop, ICON_STOP, ICON_OUTLINE))
        player_halt(app);
    if (widget_icon(app, back, ICON_REWIND, ICON_OUTLINE))
        player_seek(app, -SEEK_MS);
    if (widget_icon(app, forward, ICON_FORWARD, ICON_OUTLINE))
        player_seek(app, SEEK_MS);
}

/*
** repeat: dimmed when off, outlined for the queue, filled (with a "1")
** for the song
*/
static void modes(app_t *app, float y)
{
    settings_t const *set = &app->lib->settings;
    float shuffle[4] = {1236, y, 56, 56};
    float repeat[4] = {1308, y, 56, 56};
    int states[3] = {ICON_DIM, ICON_OUTLINE, ICON_FILLED};
    char const *repeats[3] = {"Boucle : non", "Boucle : file",
        "Boucle : titre"};

    widget_help(app, shuffle, set->shuffle ? "Aléatoire : oui"
        : "Aléatoire : non");
    widget_help(app, repeat, repeats[set->repeat]);
    if (widget_icon(app, shuffle, ICON_SHUFFLE, set->shuffle ? ICON_FILLED
            : ICON_DIM))
        player_shuffle_toggle(app);
    if (widget_icon(app, repeat, ICON_REPEAT, states[set->repeat]))
        player_repeat_next(app);
    if (set->repeat == REPEAT_ONE)
        widget_text(app, "1", (float [3]){1358, y - 8, 20}, &sfWhite);
}

static void volume(app_t *app, float y)
{
    settings_t const *set = &app->lib->settings;
    float down[4] = {1600, y, 56, 56};
    float mute[4] = {1664, y, 56, 56};
    float up[4] = {1728, y, 56, 56};
    char label[16];

    widget_help(app, mute, set->mute ? "Remettre le son" : "Couper le son");
    if (widget_icon(app, down, ICON_VOL_DOWN, ICON_OUTLINE))
        player_volume_step(app, -VOLUME_STEP);
    if (widget_icon(app, mute, set->mute ? ICON_MUTE : ICON_VOLUME,
            set->mute ? ICON_FILLED : ICON_OUTLINE))
        player_mute_toggle(app);
    if (widget_icon(app, up, ICON_VOL_UP, ICON_OUTLINE))
        player_volume_step(app, VOLUME_STEP);
    snprintf(label, sizeof(label), set->mute ? "muet" : "%d %%",
        set->volume);
    widget_text(app, label, (float [3]){1796, y + 12, 24}, &gray);
}

void library_play_modes(app_t *app)
{
    float y = 990 + app->extra_h;

    moves(app, y);
    modes(app, y);
    volume(app, y);
}
