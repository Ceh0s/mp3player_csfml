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
static int typing(app_t *app)
{
    scene_id_t scene = app->scene;

    if (app->search.focus)
        return 1;
    if (scene == RANKING)
        return app->ranking && app->ranking->reset;
    if (scene == LIBRARY)
        return app->libscene && app->libscene->sel.open;
    if (scene == STATS)
        return app->stats && app->stats->focus;
    return scene == EDIT || scene == PLAYLIST || scene == BOARDS
        || scene == DOWNLOAD;
}

/*
** the screens with pages or lists over them handle escape themselves
*/
static int own_escape(scene_id_t scene)
{
    return scene == EDIT || scene == BOARDS || scene == DOWNLOAD
        || scene == RANKING || scene == LIBRARY || scene == STATS;
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
    if (event->key.code == sfKeyEscape && !own_escape(app->scene))
        scene_back(app);
    if (event->key.code == sfKeySpace && !typing(app))
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
    app->search.shown = 0;
    app->wg->caret.shown = 0;
    scenes[app->scene].draw(app);
    sfRenderWindow_display(app->window);
}

/*
** the lists that move set the tick back to TICK_SMOOTH (scroll.c)
*/
static void update(app_t *app)
{
    app->delta_us = sfTime_asMicroseconds(sfClock_restart(app->delta));
    app->tick_loop = TICK_MS;
    app->scroll_held = 0;
    player_update(app);
    download_poll(app);
    if (scenes[app->scene].update)
        scenes[app->scene].update(app);
    autosave(app);
    window_apply_toggle(app);
}

/*
** the release of a held slider is not a click on what is under it
*/
static int released_slider(app_t *app, sfEvent const *event)
{
    return app->scroll_held && event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseLeft;
}

static void poll_events(app_t *app)
{
    sfEvent event;

    while (sfRenderWindow_pollEvent(app->window, &event)) {
        if (key_repeated(app, &event))
            continue;
        arrow_scroll(&event);
        if ((typing(app) && caret_key(app, &event))
            || search_event(app, &event))
            continue;
        if (released_slider(app, &event))
            continue;
        global_event(app, &event);
        scenes[app->scene].event(app, &event);
    }
}

int loop_run(app_t *app)
{
    wait_tick(app);
    view_mouse(app);
    poll_events(app);
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
