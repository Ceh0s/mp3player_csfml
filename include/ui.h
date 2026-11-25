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
/*
** icons of the atlas (assets/atlas.png, cells of ATLAS_CELL pixels on
** ATLAS_COLS columns): each one is drawn once in outline, then a second
** time as a filled disc (ICON_X / ICON_Y in mp3player.h)
*/
typedef enum icon_e {
    ICON_PLAY,
    ICON_PAUSE,
    ICON_PREV,
    ICON_NEXT,
    ICON_STOP,
    ICON_REWIND,
    ICON_FORWARD,
    ICON_VOL_DOWN,
    ICON_VOL_UP,
    ICON_MUTE,
    ICON_VOLUME,
    ICON_SHUFFLE,
    ICON_REPEAT,
    ICON_LIKE,
    ICON_DISLIKE,
    ICON_LIST,
    ICON_ASC,
    ICON_DESC,
    ICON_NOTE,
    ICON_STAR,
    ICON_SLIDERS,
    ICON_SHARE,
    ICON_GEAR,
    ICON_POWER,
    ICON_BACK,
    NB_ICONS
} icon_t;

/*
** how an icon is drawn: in outline, as a filled disc (a state that is
** on), or dimmed (a state that is off)
*/
typedef enum icon_state_e {
    ICON_OUTLINE,
    ICON_FILLED,
    ICON_DIM
} icon_state_t;

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

    #define TIP_LEN 256

/*
** cursor of the field receiving the keys (one at a time): buf : its
** text, pos : byte where the next character goes, first : first byte
** shown (a long text scrolls), shown : 1 if the field was drawn with
** the focus at the last frame (the arrows move the cursor only then)
*/
typedef struct caret_s {
    char *buf;
    size_t pos;
    size_t first;
    int shown;
} caret_t;

/*
** picture / circle : shapes drawing the images (square or round)
** tip : text of the hovered element, drawn last by widget_tip_draw
** clip : view of the part of the screen drawn (clip.c)
** caret : cursor of the field being typed in (text_caret.c)
** icon : sprite of the atlas drawing the icons (widget_icon.c)
*/
typedef struct widgets_s {
    sfText *text;
    sfSprite *icon;
    sfRectangleShape *box;
    sfRectangleShape *picture;
    sfCircleShape *circle;
    sfView *clip;
    char tip[TIP_LEN];
    wg_mode_t mode;
    caret_t caret;
} widgets_t;

/*
** an image loaded for the screens (only those shown are loaded);
** texture is NULL when there is no image file, so it is not looked for
** again at each tick
*/
typedef struct image_s {
    int artist;
    int id;
    int small;
    sfTexture *texture;
    struct image_s *next;
} image_t;

typedef struct ui_s {
    ui_def_t const *defs;
    sfSprite **sprites;
    float (*rects)[4];
    int *visible;
    int nb;
} ui_t;

#endif
