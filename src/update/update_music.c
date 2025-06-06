/*
** EPITECH PROJECT, 2025
** handle textures
** File description:
** home to all texture-related events
*/

#include "../../include/mp3player.h"

static void music_state(sfMusic *new, sfMusic *old)
{
    if (old)
        sfMusic_stop(old);
    if (sfMusic_getStatus(new) == sfPlaying)
        sfMusic_pause(new);
    else
        sfMusic_play(new);
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

static void locate_click(app_t **app)
{
    int idx = is_text_clicked((*app), (*app)->tx->music);
    int is_playing = get_ongoing_music((*app)->tx->music);

    if (idx != -1) {
        if (is_playing == -1)
            music_state((*app)->tx->music->mus[idx], NULL);
        else
            music_state((*app)->tx->music->mus[idx],
            (*app)->tx->music->mus[is_playing]);
    } else if (mouse_on_sprite((*app), (*app)->tx->list->sprite[1])
        && is_playing != -1)
        music_state((*app)->tx->music->mus[is_playing], NULL);
}

void update_music(app_t **app, sfEvent event)
{
    if (event.type == sfEvtMouseButtonReleased && (*app)->state == PLAYLIST
        && event.mouseButton.button == sfMouseLeft) {
        locate_click(app);
    }
}
