/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** choice of the image of a song or an artist: a field where the path of
** an image file is pasted (Ctrl+V), "Importer", and for a song
** "Pochette du mp3" to take back the cover of its file
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static void import(app_t *app, rank_t const *rank, image_input_t *input)
{
    if (image_import(rank, input->path) == 84) {
        snprintf(input->msg, sizeof(input->msg), "Impossible de lire cette "
            "image (png, jpg, bmp...)");
        return;
    }
    snprintf(input->msg, sizeof(input->msg), "Image importée");
    input->path[0] = '\0';
    image_cache_clear(app);
}

static void cover(app_t *app, rank_t const *rank, image_input_t *input)
{
    if (image_from_cover(rank) == 84) {
        snprintf(input->msg, sizeof(input->msg), "Ce mp3 n'a pas de "
            "pochette");
        return;
    }
    snprintf(input->msg, sizeof(input->msg), "Pochette du mp3 reprise");
    image_cache_clear(app);
}

/*
** pos = {x, y, width}; returns 1 if the field was clicked (the caller
** then gives it the focus)
*/
int image_box(app_t *app, rank_t const *rank, image_input_t *input,
    float const *pos)
{
    float half = (pos[2] - 10) / 2;
    float field[4] = {pos[0], pos[1] + 30, pos[2], 44};
    float left[4] = {pos[0], pos[1] + 84, rank->song ? half : pos[2], 44};
    float right[4] = {pos[0] + half + 10, pos[1] + 84, half, 44};
    int clicked = 0;

    widget_text(app, "Image : colle un chemin (Ctrl+V)",
        (float [3]){pos[0], pos[1], 20}, &sfWhite);
    clicked = widget_field(app, field, input->path, input->focused);
    if (widget_button(app, left, "Importer"))
        import(app, rank, input);
    if (rank->song && widget_button(app, right, "Pochette du mp3"))
        cover(app, rank, input);
    widget_text(app, input->msg, (float [3]){pos[0], pos[1] + 136, 18},
        &gray);
    return clicked;
}
