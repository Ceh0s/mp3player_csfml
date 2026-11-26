/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** creates, draws and destroys the sprites described by a ui_def_t table
** (same idea as disp_vert in my_world, but the sprites are created once)
*/

#include "mp3player.h"

static sfSprite *create_sprite(sfTexture *texture, float const *c)
{
    sfSprite *sprite = sfSprite_create();
    sfIntRect area = {c[4], c[5], c[6], c[7]};
    sfVector2f scale = {c[2] / c[6], c[3] / c[7]};

    if (!sprite)
        return NULL;
    sfSprite_setTexture(sprite, texture, sfFalse);
    sfSprite_setTextureRect(sprite, area);
    sfSprite_setScale(sprite, scale);
    return sprite;
}

ui_t *ui_create(sfTexture *texture, ui_def_t const *defs, int nb)
{
    ui_t *ui = calloc(1, sizeof(ui_t));

    if (!ui)
        return NULL;
    ui->defs = defs;
    ui->nb = nb;
    ui->sprites = calloc(nb, sizeof(sfSprite *));
    ui->visible = malloc(sizeof(int) * nb);
    ui->rects = malloc(sizeof(float [4]) * nb);
    if (!ui->sprites || !ui->visible || !ui->rects) {
        ui_destroy(ui);
        return NULL;
    }
    for (int i = 0; i < nb; i++) {
        ui->sprites[i] = create_sprite(texture, defs[i].rect);
        ui->visible[i] = 1;
    }
    ui_layout(ui, 0);
    return ui;
}

void ui_draw(app_t *app, ui_t *ui)
{
    sfColor idle = dark_tint(app);
    sfColor hover = {idle.r, idle.g, idle.b, 200};

    for (int i = 0; i < ui->nb; i++) {
        if (!ui->visible[i] || !ui->sprites[i])
            continue;
        if (mouse_in_rect(app, ui->rects[i]))
            sfSprite_setColor(ui->sprites[i], hover);
        else
            sfSprite_setColor(ui->sprites[i], idle);
        sfRenderWindow_drawSprite(app->window, ui->sprites[i], NULL);
    }
}

void ui_layout(ui_t *ui, float extra_h)
{
    float shift = 0;

    for (int i = 0; i < ui->nb; i++) {
        shift = 0;
        if (ui->defs[i].anchor == ANCHOR_CENTER)
            shift = extra_h / 2;
        if (ui->defs[i].anchor == ANCHOR_BOTTOM)
            shift = extra_h;
        memcpy(ui->rects[i], ui->defs[i].rect, sizeof(float [4]));
        ui->rects[i][1] += shift;
        if (ui->sprites[i])
            sfSprite_setPosition(ui->sprites[i],
                (sfVector2f){ui->rects[i][0], ui->rects[i][1]});
    }
}

void ui_show(ui_t *ui, int id, int visible)
{
    for (int i = 0; i < ui->nb; i++)
        if (ui->defs[i].id == id)
            ui->visible[i] = visible;
}

void ui_destroy(ui_t *ui)
{
    if (!ui)
        return;
    for (int i = 0; ui->sprites && i < ui->nb; i++)
        if (ui->sprites[i])
            sfSprite_destroy(ui->sprites[i]);
    free(ui->sprites);
    free(ui->visible);
    free(ui->rects);
    free(ui);
}
