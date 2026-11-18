/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** data-driven ui elements, placed in design coordinates (1920x1080)
*/

#ifndef UI_H_
    #define UI_H_

    #include <SFML/Graphics.h>

/*
** rect = {x, y, w, h, tx, ty, tw, th}
** x, y, w, h     : position and size on screen, in design pixels
** tx, ty, tw, th : area of the texture (atlas) to display
*/
typedef enum anchor_e {
    ANCHOR_TOP,
    ANCHOR_CENTER,
    ANCHOR_BOTTOM
} anchor_t;

/*
** anchor : on screens taller than 16:9, the extra height is added
** between the top and the bottom of the design: elements anchored at
** the bottom move down by all of it, centered ones by half of it.
*/
typedef struct ui_def_s {
    int id;
    anchor_t anchor;
    float rect[8];
} ui_def_t;

/*
** Widgets are drawn in "immediate mode": a single function describes a
** screen, it is called with mode WG_DRAW to draw it, and with WG_CLICK
** when the mouse is released (widget_button then returns 1 for the
** clicked button, and switches to WG_IDLE so nothing else reacts).
*/
typedef enum wg_mode_e {
    WG_DRAW,
    WG_CLICK,
    WG_IDLE
} wg_mode_t;

typedef struct widgets_s {
    sfText *text;
    sfRectangleShape *box;
    wg_mode_t mode;
} widgets_t;

typedef struct ui_s {
    ui_def_t const *defs;
    sfSprite **sprites;
    float (*rects)[4];
    int *visible;
    int nb;
} ui_t;

#endif
