/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** search field of the screens with a long list (library, duplicates,
** rankings, list of the download): a click on it gives it the keys,
** Enter or a click elsewhere takes them back, Escape empties it first.
** The text (app->search) is compared without case to the titles and
** artists, and emptied when the screen changes (the library takes its
** own back: library_keep.c).
*/

#include "mp3player.h"

/*
** an empty field shows a magnifying glass (drawn only, not a button)
*/
void search_field(app_t *app, float const *rect)
{
    search_t *search = &app->search;
    float side = rect[3] - 12;
    float glass[4] = {rect[0] + 8, rect[1] + 6, side, side};

    widget_field(app, rect, search->text, search->focus);
    if (app->wg->mode != WG_DRAW)
        return;
    search->shown = 1;
    memcpy(search->rect, rect, sizeof(search->rect));
    if (search->text[0] == '\0' && !search->focus)
        widget_icon(app, glass, ICON_SEARCH, ICON_DIM);
}

/*
** keys of the field; returns 1 if the event is used here
*/
static int search_key(app_t *app, search_t *search, sfEvent const *event)
{
    sfUint32 code = event->text.unicode;
    int pressed = event->type == sfEvtKeyPressed;
    int escape = pressed && event->key.code == sfKeyEscape;
    int enter = pressed && event->key.code == sfKeyEnter;

    if (escape && search->text[0] != '\0') {
        search->text[0] = '\0';
        search->changed = 1;
        return 1;
    }
    if (!search->focus)
        return 0;
    if (escape || enter)
        search->focus = 0;
    if (event->type == sfEvtTextEntered)
        search->changed |= text_input(app, search->text, code);
    return pressed || event->type == sfEvtTextEntered;
}

/*
** returns 1 if the event was used by the field (the screen ignores it)
*/
int search_event(app_t *app, sfEvent const *event)
{
    search_t *search = &app->search;

    if (!search->shown) {
        search->focus = 0;
        return 0;
    }
    if (event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseLeft)
        search->focus = mouse_in_rect(app, search->rect);
    return search_key(app, search, event);
}

/*
** 1 if the text matches (always when the field is empty)
*/
int search_text(app_t *app, char const *text)
{
    if (app->search.text[0] == '\0')
        return 1;
    return text && str_contains(text, app->search.text);
}

int search_song(app_t *app, song_t const *song)
{
    return song_matches(song, app->search.text);
}
