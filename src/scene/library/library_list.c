/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** scrollable list of the songs of the library screen
*/

#include "mp3player.h"

static char *row_label(song_t const *song)
{
    char const *title = song_title(song);
    char *label = malloc(strlen(title) + 4
        + (song->artists ? strlen(song->artists) : 0));

    if (!label)
        return NULL;
    strcpy(label, title);
    if (song->artists) {
        strcat(label, " - ");
        strcat(label, song->artists);
    }
    return label;
}

int list_create(app_t *app)
{
    library_t *lib = app->lib;
    sfText **titles = calloc(lib->nb + 1, sizeof(sfText *));
    char *label = NULL;

    app->libscene->titles = titles;
    if (!titles)
        return 84;
    for (int i = 0; i < lib->nb; i++) {
        titles[i] = text_create(app, 36, LIST_X, LIST_Y);
        label = row_label(lib->playlist[i]);
        if (!titles[i] || !label) {
            free(label);
            return 84;
        }
        text_set(titles[i], label);
        free(label);
    }
    return 0;
}

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
    int max = app->lib->nb - app->libscene->rows;
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
        if (i == app->player->index)
            sfText_setFillColor(titles[i], sfYellow);
        else
            sfText_setFillColor(titles[i], sfWhite);
        if (i != app->player->index && list_click(app) == i)
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
