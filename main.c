/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** main of wolf3d
*/

#include "include/mp3player.h"

int main(void)
{
    app_t *app = init_all();
    sfEvent event;

    if (!app->win->window)
        return destroy_all(app, 84);
    while (sfRenderWindow_isOpen(app->win->window)) {
        if (handle_events(&app, &event) == 84)
            return destroy_all(app, 84);
        draw_all(app);
        sfRenderWindow_display(app->win->window);
    }
    return destroy_all(app, 0);
}
