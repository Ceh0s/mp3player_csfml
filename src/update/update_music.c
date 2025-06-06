/*
** EPITECH PROJECT, 2025
** handle textures
** File description:
** home to all texture-related events
*/

#include "../../include/mp3player.h"

static void music_state(int new, int old, app_t **app)
{
    if (old >= 0) {
        sfMusic_stop((*app)->tx->music->mus[old]);
        sfText_setFillColor((*app)->tx->music->text[old], sfWhite);
    }
    if (sfMusic_getStatus((*app)->tx->music->mus[new]) == sfPlaying) {
        sfMusic_pause((*app)->tx->music->mus[new]);
        (*app)->tx->music->paused = 1;
    } else {
        sfMusic_play((*app)->tx->music->mus[new]);
        sfText_setFillColor((*app)->tx->music->text[new], sfYellow);
        (*app)->tx->music->paused = 0;
    }
}

static int is_text_clicked(app_t *app, music_t *music)
{
    for (int i = 0; music->text[i]; i++)
        if (mouse_on_text(app, music->text[i]) == 1)
            return i;
    return -1;
}

static int get_ongoing_music(music_t *music)
{
    sfSoundStatus status;

    for (int i = 0; music->mus[i]; i++) {
        status = sfMusic_getStatus(music->mus[i]);
        if (status == sfPlaying || status == sfPaused)
            return i;
    }
    return -1;
}

static void skip_buttons(app_t **app, int is_playing)
{
    if (mouse_on_sprite((*app), (*app)->tx->list->sprite[6])
        && is_playing > 0) {
        music_state(is_playing - 1, is_playing, app);
    } else if (mouse_on_sprite((*app), (*app)->tx->list->sprite[4])
        && (*app)->tx->music->nb - 1 != is_playing)
        music_state(is_playing + 1, is_playing, app);
}

static void pause_resume(app_t **app, int idx, int is_playing)
{
    if (idx != -1) {
        if (is_playing == -1)
            music_state(idx, -1, app);
        else
            music_state(idx, is_playing, app);
    } else if (mouse_on_sprite((*app), (*app)->tx->list->sprite[1])
        && is_playing != -1)
        music_state(is_playing, -1, app);
}

void update_music(app_t **app, sfEvent event)
{
    int idx = is_text_clicked((*app), (*app)->tx->music);
    int is_playing = get_ongoing_music((*app)->tx->music);

    if (event.type == sfEvtMouseButtonReleased && (*app)->state == PLAYLIST
        && event.mouseButton.button == sfMouseLeft) {
        pause_resume(app, idx, is_playing);
        if (is_playing != -1)
            skip_buttons(app, is_playing);
    }
}
