/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** settings screen, opened from the home screen: each setting is saved
** at once in settings.tsv
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

/*
** texts of a switch: on, off, detail shown on its right
*/
static char const *const help[3] = {
    "Aide au survol : activée",
    "Aide au survol : désactivée",
    ""
};
static char const *const ellipsis[3] = {
    "Points de suspension : oui",
    "Points de suspension : non",
    "Coupe les titres trop longs."
};

int settings_init(app_t *app)
{
    float panel[4] = {87, 110, 1745, PANEL_H};
    sfColor dark = {0, 0, 0, 140};

    app->settings = calloc(1, sizeof(setscene_t));
    if (!app->settings)
        return 84;
    app->settings->bg = background_create(BG_BLUR_PATH,
        &app->settings->bg_texture);
    app->settings->panel = rect_create(panel, &dark);
    if (!app->settings->bg || !app->settings->panel)
        return 84;
    return 0;
}

/*
** a switch of the settings, and what it does on its right;
** texts = {label when on, label when off, detail}
*/
static void switch_row(app_t *app, float y, int *value,
    char const *const *texts)
{
    float rect[4] = {130, y, 520, 56};

    widget_text(app, texts[2], (float [3]){680, y + 14, 24}, &gray);
    if (!widget_button(app, rect, *value ? texts[0] : texts[1]))
        return;
    *value = !*value;
    db_save_settings(app->lib);
}

static void view(app_t *app)
{
    settings_t *set = &app->lib->settings;

    widget_text(app, "Paramètres", (float [3]){130, 125, 40}, &sfWhite);
    switch_row(app, 250, &set->help, help);
    switch_row(app, 330, &set->ellipsis, ellipsis);
    if (widget_back(app))
        scene_back(app);
}

void settings_event(app_t *app, sfEvent *event)
{
    if (event->type != sfEvtMouseButtonReleased
        || event->mouseButton.button != sfMouseLeft)
        return;
    app->wg->mode = WG_CLICK;
    view(app);
    app->wg->mode = WG_DRAW;
}

void settings_draw(app_t *app)
{
    sfRenderWindow_drawSprite(app->window, app->settings->bg, NULL);
    sfRenderWindow_drawRectangleShape(app->window, app->settings->panel,
        NULL);
    app->wg->mode = WG_DRAW;
    view(app);
    widget_tip_draw(app);
}

void settings_layout(app_t *app)
{
    sfVector2f size = {1745, PANEL_H + app->extra_h};

    background_layout(app, app->settings->bg);
    sfRectangleShape_setSize(app->settings->panel, size);
}

void settings_destroy(app_t *app)
{
    if (!app->settings)
        return;
    if (app->settings->bg)
        sfSprite_destroy(app->settings->bg);
    if (app->settings->bg_texture)
        sfTexture_destroy(app->settings->bg_texture);
    if (app->settings->panel)
        sfRectangleShape_destroy(app->settings->panel);
    free(app->settings);
    app->settings = NULL;
}
