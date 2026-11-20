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
    #include "download.h"

/*
** arena : ranking, kind and category chosen in the battle and rankings
** screens, kept when going from one to the other
*/
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
    sfTexture *no_cover;
    image_t *images;
    int show_images;
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
    pledit_t *pledit;
    boards_t *boards;
    dlscene_t *dlscene;
    song_t *edit_song;
    playlist_t *edit_playlist;
    int library_tab;
    arena_t arena;
    widgets_t *wg;
    library_t *lib;
    player_t *player;
    download_t download;
};

#endif
