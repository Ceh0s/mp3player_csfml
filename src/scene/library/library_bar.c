/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** bottom bar of the library screen: current song, progress and time
*/

#include "mp3player.h"

static const float bar_rect[4] = {0, 926, VIEW_W, 154};
static const float progress_rect[4] = {460, 944, 1000, 6};
static const float seek_rect[4] = {460, 930, 1000, 34};

int bar_create(app_t *app)
{
    libscene_t *scene = app->libscene;
    sfColor bar = {40, 40, 40, 235};
    sfColor empty = {110, 110, 110, 255};
    sfColor full = {255, 200, 0, 255};

    scene->bar = rect_create(bar_rect, &bar);
    scene->progress_bg = rect_create(progress_rect, &empty);
    scene->progress = rect_create(progress_rect, &full);
    scene->now_title = text_create(app, 30, 40, 952);
    scene->now_artist = text_create(app, 22, 40, 996);
    scene->time = text_create(app, 22, 1480, 932);
    scene->hint = text_create(app, 22, 200, 34);
    if (!scene->bar || !scene->progress_bg || !scene->progress
        || !scene->now_title || !scene->now_artist || !scene->time
        || !scene->hint)
        return 84;
    text_set(scene->hint, "Clic droit sur une musique pour la modifier");
    sfText_setFillColor(scene->hint, (sfColor){200, 200, 200, 255});
    sfText_setFillColor(scene->now_artist, (sfColor){190, 190, 190, 255});
    return 0;
}

int bar_seek(app_t *app)
{
    sfMusic *music = app->player->music;
    float seek[4] = {seek_rect[0], seek_rect[1] + app->extra_h,
        seek_rect[2], seek_rect[3]};
    float ratio = (app->mouse.x - seek[0]) / seek[2];
    sfTime duration;

    if (!music || !mouse_in_rect(app, seek))
        return 0;
    duration = sfMusic_getDuration(music);
    sfMusic_setPlayingOffset(music,
        sfMicroseconds(duration.microseconds * ratio));
    return 1;
}

static void update_labels(app_t *app)
{
    libscene_t *scene = app->libscene;
    song_t *song = NULL;

    scene->shown = app->player->index;
    if (scene->shown < 0) {
        text_set(scene->now_title, app->lib->nb > 0 ? "Aucune lecture"
            : "Aucune musique dans le dossier music/");
        text_set(scene->now_artist, "");
        return;
    }
    song = app->lib->playlist[scene->shown];
    text_set(scene->now_title, song_title(song));
    text_set(scene->now_artist, song->artists ? song->artists : "");
}

void bar_update(app_t *app)
{
    sfMusic *music = app->player->music;
    sfVector2f size = {0, progress_rect[3]};
    long now = 0;
    long total = 0;
    char buf[2][16];
    char label[40];

    if (app->libscene->shown != app->player->index)
        update_labels(app);
    if (music) {
        now = sfTime_asMilliseconds(sfMusic_getPlayingOffset(music));
        total = sfTime_asMilliseconds(sfMusic_getDuration(music));
    }
    if (total > 0)
        size.x = progress_rect[2] * now / total;
    sfRectangleShape_setSize(app->libscene->progress, size);
    format_time(buf[0], sizeof(buf[0]), now);
    format_time(buf[1], sizeof(buf[1]), total);
    snprintf(label, sizeof(label), "%s / %s", buf[0], buf[1]);
    text_set(app->libscene->time, label);
}

/*
** the bar is anchored at the bottom: it moves down by the extra height,
** and takes the whole width of the view (wider on 21:9 screens)
*/
void bar_layout(app_t *app)
{
    libscene_t *scene = app->libscene;
    float y = app->extra_h;

    sfRectangleShape_setPosition(scene->bar,
        (sfVector2f){app->view_rect.left, bar_rect[1] + y});
    sfRectangleShape_setSize(scene->bar,
        (sfVector2f){app->view_rect.width, bar_rect[3]});
    sfRectangleShape_setPosition(scene->progress_bg,
        (sfVector2f){progress_rect[0], progress_rect[1] + y});
    sfRectangleShape_setPosition(scene->progress,
        (sfVector2f){progress_rect[0], progress_rect[1] + y});
    sfText_setPosition(scene->now_title, (sfVector2f){40, 952 + y});
    sfText_setPosition(scene->now_artist, (sfVector2f){40, 996 + y});
    sfText_setPosition(scene->time, (sfVector2f){1480, 932 + y});
}

void bar_draw(app_t *app)
{
    libscene_t *scene = app->libscene;

    sfRenderWindow_drawRectangleShape(app->window, scene->bar, NULL);
    sfRenderWindow_drawRectangleShape(app->window, scene->progress_bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, scene->progress, NULL);
    sfRenderWindow_drawText(app->window, scene->now_title, NULL);
    sfRenderWindow_drawText(app->window, scene->now_artist, NULL);
    sfRenderWindow_drawText(app->window, scene->time, NULL);
    sfRenderWindow_drawText(app->window, scene->hint, NULL);
}
