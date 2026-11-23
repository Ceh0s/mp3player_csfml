/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** queue of the player: the songs of the playlist being played, in its
** order ("Tous les titres" = every present song), only those matching
** the search of the library when the song was chosen (filter), in a
** random order if asked (player_shuffle.c)
*/

#include "mp3player.h"

/*
** keep = 1 : a random order already drawn is kept (rebuilt after an
** edit), else it is drawn again with the current song first
*/
static void order(app_t *app, song_t **queue, int nb, int keep)
{
    player_t *player = app->player;

    if (!app->lib->settings.shuffle)
        return;
    if (keep && player->queue && queue_keep_order(queue, nb, player) == 0)
        return;
    queue_shuffle(queue, nb, player->song);
}

/*
** the current song keeps playing, its place in the new queue is found;
** returns 84 if the queue cannot be allocated
*/
static int queue_build(app_t *app, int keep)
{
    player_t *player = app->player;
    playlist_t *playlist = playlist_by_id(app->lib, player->playlist_id);
    song_t **queue = malloc(sizeof(song_t *) * (app->lib->nb + 1));
    int nb = 0;

    if (!queue)
        return 84;
    if (!playlist)
        player->playlist_id = 0;
    nb = playlist_sorted_view(app->lib, playlist, queue);
    nb = view_filter(queue, nb, player->filter);
    order(app, queue, nb, keep);
    free(player->queue);
    player->queue = queue;
    player->nb = nb;
    player->index = -1;
    for (int i = 0; player->song && i < nb; i++)
        if (queue[i] == player->song)
            player->index = i;
    return 0;
}

/*
** the next songs will be those of this playlist (0 = every song)
*/
void player_queue(app_t *app, int playlist_id, char const *filter)
{
    player_t *player = app->player;

    if (player->queue && player->playlist_id == playlist_id
        && strcmp(player->filter, filter) == 0)
        return;
    player->playlist_id = playlist_id;
    snprintf(player->filter, NAME_LEN, "%s", filter);
    queue_build(app, 0);
}

/*
** the present songs or the playlists changed (edit, merge): the queue
** is rebuilt; the song stops only if its file is not there anymore
** (removed from its playlist, it ends then the queue starts again)
*/
void player_resync(app_t *app)
{
    song_t *song = app->player->song;

    if (queue_build(app, 1) == 84 || (song && !song->present))
        player_stop(app, END_SKIP);
}

/*
** "Aléatoire": the queue is drawn again (or sorted again), saved
*/
void player_shuffle_toggle(app_t *app)
{
    app->lib->settings.shuffle = !app->lib->settings.shuffle;
    queue_build(app, 0);
    db_save_settings(app->lib);
}

/*
** "Boucle": off, then the queue, then the same song
*/
void player_repeat_next(app_t *app)
{
    settings_t *settings = &app->lib->settings;

    settings->repeat = (settings->repeat + 1) % 3;
    db_save_settings(app->lib);
}
