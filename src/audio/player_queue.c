/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** queue of the player: the songs of the playlist being played, in its
** order ("Tous les titres" = every present song), only those matching
** the search of the library when the song was chosen (filter)
*/

#include "mp3player.h"

/*
** the current song keeps playing, its place in the new queue is found;
** returns 84 if the queue cannot be allocated
*/
static int queue_build(app_t *app)
{
    player_t *player = app->player;
    playlist_t *playlist = playlist_by_id(app->lib, player->playlist_id);

    free(player->queue);
    player->nb = 0;
    player->index = -1;
    player->queue = malloc(sizeof(song_t *) * (app->lib->nb + 1));
    if (!player->queue)
        return 84;
    if (!playlist)
        player->playlist_id = 0;
    player->nb = playlist_sorted_view(app->lib, playlist, player->queue);
    player->nb = view_filter(player->queue, player->nb, player->filter);
    for (int i = 0; player->song && i < player->nb; i++)
        if (player->queue[i] == player->song)
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
    queue_build(app);
}

/*
** the present songs or the playlists changed (edit, merge): the queue
** is rebuilt; the song stops only if its file is not there anymore
** (removed from its playlist, it ends then the queue starts again)
*/
void player_resync(app_t *app)
{
    song_t *song = app->player->song;

    if (queue_build(app) == 84 || (song && !song->present))
        player_stop(app, END_SKIP);
}
