/*
** EPITECH PROJECT, 2025
** handle textures
** File description:
** home to all texture-related events
*/

#include "../../include/mp3player.h"

static void music_state(music_t *music)
{
    sfSoundStatus sstatus = sfMusic_getStatus(music->mus[0]);
    
    if (sstatus == sfStopped || sstatus == sfPaused)
        sfMusic_play(music->mus[0]);
    if (sstatus == sfPlaying)
        sfMusic_pause(music->mus[0]);
}

void update_music(app_t **app, sfEvent event)
{
    if (event.type == sfEvtMouseButtonReleased && (*app)->state == PLAYLIST
        && event.mouseButton.button == sfMouseLeft &&
        mouse_on_sprite(*app, (*app)->tx->list->sprite[1]) == 1)
        music_state((*app)->tx->music);
}
