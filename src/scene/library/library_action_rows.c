/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** rows of the actions window of the library where text is typed: the
** text inserted in the titles (where, and how the first title becomes)
** and the image given to every song; and the keys typed in them
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

static void where_button(app_t *app, float const *rect, char const *name,
    int where)
{
    selection_t *sel = &app->libscene->sel;
    char text[32];

    snprintf(text, sizeof(text), sel->where == where ? "> %s <" : "%s",
        name);
    if (widget_button(app, rect, text))
        sel->where = where;
}

static void preview(app_t *app, float y)
{
    selection_t *sel = &app->libscene->sel;
    song_t *first = sel->nb > 0 ? select_song(app, 0) : NULL;
    char title[NAME_LEN];
    char text[NAME_LEN + 32];

    if (!first || sel->text[0] == '\0')
        return;
    insert_title(sel, song_title(first), title);
    snprintf(text, sizeof(text), "Exemple : %s", title);
    utf8_cut(title, text, 90);
    widget_text(app, title, (float [3]){520, y + 12, 22}, &gray);
}

void library_insert_rows(app_t *app, float y)
{
    selection_t *sel = &app->libscene->sel;
    float after[4] = {1545, y, 110, 52};

    widget_text(app, "Insérer", (float [3]){300, y + 12, 26}, &gray);
    if (widget_field(app, (float [4]){520, y, 520, 52}, sel->text,
            sel->focus == SEL_FOCUS_TEXT))
        sel->focus = SEL_FOCUS_TEXT;
    where_button(app, (float [4]){1055, y, 140, 52}, "Début", SEL_START);
    where_button(app, (float [4]){1205, y, 120, 52}, "Fin", SEL_END);
    where_button(app, (float [4]){1335, y, 200, 52}, "Après n car.",
        SEL_AFTER);
    if (sel->where == SEL_AFTER && widget_field(app, after, sel->after,
            sel->focus == SEL_FOCUS_AFTER))
        sel->focus = SEL_FOCUS_AFTER;
    preview(app, y + 70);
    if (widget_button(app, (float [4]){1455, y + 70, 200, 52}, "Insérer"))
        bulk_insert(app);
}

void library_image_row(app_t *app, float y)
{
    selection_t *sel = &app->libscene->sel;

    widget_text(app, "Image", (float [3]){300, y + 12, 26}, &gray);
    if (widget_field(app, (float [4]){520, y, 700, 52}, sel->image,
            sel->focus == SEL_FOCUS_IMAGE))
        sel->focus = SEL_FOCUS_IMAGE;
    if (widget_button(app, (float [4]){1235, y, 190, 52}, "Importer"))
        bulk_images(app, 0);
    if (widget_button(app, (float [4]){1440, y, 215, 52}, "Pochette mp3"))
        bulk_images(app, 1);
}

/*
** the number of characters only takes digits
*/
void library_actions_key(app_t *app, sfUint32 code)
{
    selection_t *sel = &app->libscene->sel;
    int digit = code >= '0' && code <= '9';

    if (sel->focus == SEL_FOCUS_TEXT)
        text_input(app, sel->text, code);
    if (sel->focus == SEL_FOCUS_IMAGE)
        text_input(app, sel->image, code);
    if (sel->focus == SEL_FOCUS_AFTER && (code == 8 || digit))
        text_input_n(app, sel->after, sizeof(sel->after), code);
}
