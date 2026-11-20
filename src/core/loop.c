/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** main loop running at a fixed tick (from wolf3d's update_events):
** the program sleeps between two ticks instead of looping for nothing
*/

#include "mp3player.h"

static void wait_tick(app_t *app)
{
    long elapsed = sfTime_asMilliseconds(sfClock_getElapsedTime(app->ticks));

    if (elapsed < app->tick_loop)
        sfSleep(sfMilliseconds(app->tick_loop - elapsed));
    sfClock_restart(app->ticks);
}

/*
** the screens where text is typed handle escape and space themselves
*/
static int typing(scene_id_t scene)
{
    return scene == EDIT || scene == PLAYLIST || scene == BOARDS;
}

static void global_event(app_t *app, sfEvent *event)
{
    if (event->type == sfEvtClosed)
        sfRenderWindow_close(app->window);
    if (event->type == sfEvtResized)
        view_update(app);
    if (event->type != sfEvtKeyPressed)
        return;
    if (event->key.code == sfKeyF11)
        app->toggle_fullscreen = 1;
    if (event->key.code == sfKeyEscape && app->scene != EDIT
        && app->scene != BOARDS)
        scene_back(app);
    if (event->key.code == sfKeySpace && !typing(app->scene))
        player_toggle(app);
}

static void autosave(app_t *app)
{
    app->save_us += app->delta_us;
    if (app->save_us < AUTOSAVE_US)
        return;
    app->save_us = 0;
    db_save_all(app->lib);
}

static void draw(app_t *app)
{
    sfRenderWindow_clear(app->window, sfBlack);
    scenes[app->scene].draw(app);
    sfRenderWindow_display(app->window);
}

static void update(app_t *app)
{
    app->delta_us = sfTime_asMicroseconds(sfClock_restart(app->delta));
    player_update(app);
    if (scenes[app->scene].update)
        scenes[app->scene].update(app);
    autosave(app);
    window_apply_toggle(app);
}

int loop_run(app_t *app)
{
    sfEvent event;

    wait_tick(app);
    view_mouse(app);
    while (sfRenderWindow_pollEvent(app->window, &event)) {
        global_event(app, &event);
        scenes[app->scene].event(app, &event);
    }
    if (signals_pending())
        sfRenderWindow_close(app->window);
    update(app);
    if (scene_switch(app) == 84)
        return 84;
    if (app->relayout) {
        scenes[app->scene].layout(app);
        app->relayout = 0;
    }
    if (sfRenderWindow_isOpen(app->window))
        draw(app);
    return 0;
}
