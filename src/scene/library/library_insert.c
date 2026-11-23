/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** actions on the titles of the songs selected in the library: the
** title suggestions applied, and a text inserted at the start, at the
** end or after a number of characters
*/

#include "mp3player.h"

/*
** bytes taken by the n first characters of text (utf-8)
*/
static int char_offset(char const *text, int n)
{
    int pos = 0;

    while (text[pos] && n > 0) {
        pos++;
        while (((unsigned char)text[pos] & 0xC0) == 0x80)
            pos++;
        n--;
    }
    return pos;
}

/*
** out = title with sel->text inserted where chosen
*/
void insert_title(selection_t const *sel, char const *title, char *out)
{
    int at = 0;

    if (sel->where == SEL_END)
        at = strlen(title);
    if (sel->where == SEL_AFTER)
        at = char_offset(title, atoi(sel->after));
    snprintf(out, NAME_LEN, "%.*s%s%s", at, title, sel->text, title + at);
}

void bulk_insert(app_t *app)
{
    selection_t *sel = &app->libscene->sel;
    char title[NAME_LEN];
    int done = 0;

    if (sel->text[0] == '\0')
        return;
    for (int i = 0; i < sel->nb; i++) {
        if (!select_song(app, i))
            continue;
        insert_title(sel, song_title(select_song(app, i)), title);
        song_set_title(select_song(app, i), title);
        done++;
    }
    library_changed(app);
    library_reorder(app, 1);
    snprintf(sel->msg, sizeof(sel->msg), "Texte inséré dans %d titre(s).",
        done);
}

void bulk_suggest(app_t *app)
{
    selection_t *sel = &app->libscene->sel;
    int done = 0;

    for (int i = 0; i < sel->nb; i++)
        if (select_song(app, i))
            done += suggest_apply_song(app->lib, select_song(app, i)) > 0;
    library_changed(app);
    library_reorder(app, 1);
    snprintf(sel->msg, sizeof(sel->msg), "Suggestions appliquées à %d "
        "titre(s) sur %d.", done, sel->nb);
}
