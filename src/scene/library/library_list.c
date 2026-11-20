/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scrollable list of the songs of the library screen
*/

#include "mp3player.h"

int list_click(app_t *app)
{
    sfText **titles = app->libscene->titles;
    int scroll = app->libscene->scroll;
    int rows = app->libscene->rows;
    sfFloatRect box;

    for (int i = scroll; titles[i] && i < scroll + rows; i++) {
        box = sfText_getGlobalBounds(titles[i]);
        if (sfFloatRect_contains(&box, app->mouse.x, app->mouse.y))
            return i;
    }
    return -1;
}

void list_scroll(app_t *app, float delta)
{
    int max = app->libscene->nb_view - app->libscene->rows;
    int scroll = app->libscene->scroll - (int)delta;

    if (scroll > max)
        scroll = max;
    if (scroll < 0)
        scroll = 0;
    app->libscene->scroll = scroll;
}

void list_update(app_t *app)
{
    sfText **titles = app->libscene->titles;
    int scroll = app->libscene->scroll;
    int rows = app->libscene->rows;
    sfColor hover = {200, 200, 255, 255};
    sfVector2f pos = {LIST_X, 0};

    for (int i = scroll; titles[i] && i < scroll + rows; i++) {
        pos.y = LIST_Y + (i - scroll) * LIST_STEP;
        sfText_setPosition(titles[i], pos);
        if (app->libscene->view[i] == app->player->song)
            sfText_setFillColor(titles[i], sfYellow);
        else
            sfText_setFillColor(titles[i], sfWhite);
        if (app->libscene->view[i] != app->player->song
            && list_click(app) == i)
            sfText_setFillColor(titles[i], hover);
    }
}

void list_draw(app_t *app)
{
    sfText **titles = app->libscene->titles;
    int scroll = app->libscene->scroll;
    int rows = app->libscene->rows;

    for (int i = scroll; titles[i] && i < scroll + rows; i++)
        sfRenderWindow_drawText(app->window, titles[i], NULL);
}
