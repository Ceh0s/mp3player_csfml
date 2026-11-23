/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** random order of the queue (settings.shuffle): the song playing goes
** first; when the queue is built again after an edit, the order already
** drawn is kept (the songs gone leave it, the new ones go at the end)
*/

#include "mp3player.h"

/*
** Fisher-Yates: each place takes a song drawn among those left
*/
static void shuffle(song_t **songs, int nb)
{
    song_t *swap = NULL;
    int other = 0;

    for (int i = nb - 1; i > 0; i--) {
        other = rand() % (i + 1);
        swap = songs[i];
        songs[i] = songs[other];
        songs[other] = swap;
    }
}

/*
** first (if in the queue) is put at its start
*/
void queue_shuffle(song_t **queue, int nb, song_t const *first)
{
    shuffle(queue, nb);
    for (int i = 1; first && i < nb; i++) {
        if (queue[i] != first)
            continue;
        queue[i] = queue[0];
        queue[0] = (song_t *)first;
        return;
    }
}

static int has(song_t *const *songs, int nb, song_t const *song)
{
    for (int i = 0; i < nb; i++)
        if (songs[i] == song)
            return 1;
    return 0;
}

/*
** the songs of old still in fresh, in the order of old; returns their
** number
*/
static int kept_order(song_t **order, song_t *const *fresh, int nb,
    player_t const *old)
{
    int done = 0;
    song_t *song = NULL;

    for (int i = 0; i < old->nb; i++) {
        song = old->queue[i];
        if (has(fresh, nb, song) && !has(order, done, song)) {
            order[done] = song;
            done++;
        }
    }
    return done;
}

/*
** fresh (nb songs, sorted) takes the order of the queue of old, the
** songs not in it are drawn after them; returns 84 if the memory is
** missing
*/
int queue_keep_order(song_t **fresh, int nb, player_t const *old)
{
    song_t **order = malloc(sizeof(song_t *) * (nb + 1));
    int done = 0;
    int kept = 0;

    if (!order)
        return 84;
    done = kept_order(order, fresh, nb, old);
    kept = done;
    for (int i = 0; i < nb; i++)
        if (!has(order, kept, fresh[i])) {
            order[done] = fresh[i];
            done++;
        }
    shuffle(order + kept, done - kept);
    memcpy(fresh, order, sizeof(song_t *) * done);
    free(order);
    return 0;
}
