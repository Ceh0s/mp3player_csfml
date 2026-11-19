/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** main struct pointing to all the others
*/

#ifndef APP_H_
    #define APP_H_

    #include <SFML/Graphics.h>
    #include "scene.h"
    #include "song.h"
    #include "player.h"

struct app_s {
    sfRenderWindow *window;
    sfView *view;
    sfFloatRect view_rect;
    float extra_h;
    int relayout;
    int fullscreen;
    int toggle_fullscreen;
    sfFont *font;
    sfTexture *atlas;
    sfVector2f mouse;
    sfClock *ticks;
    sfClock *delta;
    long delta_us;
    long save_us;
    long tick_loop;
    scene_id_t scene;
    scene_id_t next_scene;
    scene_id_t path[NB_SCENES];
    int path_len;
    home_t *home;
    libscene_t *libscene;
    edit_t *edit;
    battle_t *battle;
    ranking_t *ranking;
    song_t *edit_song;
    widgets_t *wg;
    library_t *lib;
    player_t *player;
};

#endif
