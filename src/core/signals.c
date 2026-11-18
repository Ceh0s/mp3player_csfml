/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** clean exit on Ctrl+C (SIGINT), kill (SIGTERM) or closed terminal (SIGHUP)
** These signals are blocked: instead of killing the program they stay
** "pending", and the main loop checks every tick if one arrived.
** This way no handler and no global variable are needed.
*/

#include "mp3player.h"
#include <signal.h>

static void fill_set(sigset_t *set)
{
    sigemptyset(set);
    sigaddset(set, SIGINT);
    sigaddset(set, SIGTERM);
    sigaddset(set, SIGHUP);
}

/*
** Must be called before anything else: the threads created later
** (audio, graphics driver) inherit this mask, so none of them can
** receive the signal and be killed by it.
*/
int signals_block(void)
{
    sigset_t set;

    fill_set(&set);
    return sigprocmask(SIG_BLOCK, &set, NULL);
}

int signals_pending(void)
{
    sigset_t pending;

    if (sigpending(&pending) != 0)
        return 0;
    return sigismember(&pending, SIGINT) || sigismember(&pending, SIGTERM)
        || sigismember(&pending, SIGHUP);
}
