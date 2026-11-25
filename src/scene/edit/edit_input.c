/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** keyboard input of the edit screen and lists filtered by what is typed
*/

#include "mp3player.h"
#include <strings.h>

static char *notes_buffer(edit_t *edit)
{
    period_row_t *row = &edit->notes.rows[edit->notes.row];

    if (edit->focus == FOCUS_PERIOD)
        return edit->notes.side ? row->to : row->from;
    return edit->focus == FOCUS_FOUND ? edit->notes.found
        : edit->notes.date;
}

/*
** page "Découverte": a date of a stay only takes digits and '-', tab
** goes from a free field to the other one, or to the other date
*/
static void notes_text(app_t *app, edit_t *edit, sfUint32 code)
{
    char *buf = notes_buffer(edit);

    edit->notes.msg[0] = '\0';
    if (code == 9 && edit->focus == FOCUS_PERIOD)
        edit->notes.side = edit->notes.rows[edit->notes.row].old.end
            == PERIOD_OPEN ? 0 : !edit->notes.side;
    else if (code == 9)
        edit->focus = edit->focus == FOCUS_FOUND ? FOCUS_FOUND_DATE
            : FOCUS_FOUND;
    if (edit->focus != FOCUS_PERIOD)
        text_input(app, buf, code);
    else if (code == 8 || code == '-' || (code >= '0' && code <= '9'))
        text_input_n(app, buf, PERIOD_DATE_LEN, code);
}

static char *focused_buffer(edit_t *edit)
{
    if (edit->focus == FOCUS_RENAME && edit->mode == EDIT_ARTIST)
        return edit->rename;
    if (edit->focus == FOCUS_IMAGE && (edit->mode == EDIT_MAIN
            || edit->mode == EDIT_ARTIST))
        return edit->image.path;
    if (edit->mode == EDIT_SEARCH)
        return edit->search;
    if (edit->mode != EDIT_MAIN)
        return NULL;
    return edit->focus == FOCUS_TITLE ? edit->title : edit->input;
}

/*
** 9 = tab (switch field), 13 = enter (add the typed artist),
** the others are typed in the focused field (see text_input.c)
*/
void edit_text_entered(app_t *app, sfUint32 code)
{
    edit_t *edit = app->edit;
    char *buf = focused_buffer(edit);

    if (edit->mode == EDIT_NOTES) {
        notes_text(app, edit, code);
        return;
    }
    if (code == 9 && edit->mode == EDIT_MAIN && edit->focus != FOCUS_IMAGE)
        edit->focus = edit->focus == FOCUS_TITLE ? FOCUS_ARTIST : FOCUS_TITLE;
    if (code == 13 && edit->mode == EDIT_ARTIST && edit->focus == FOCUS_RENAME)
        edit_rename_artist(app);
    if (code == 13 && edit->mode == EDIT_MAIN && edit->focus == FOCUS_ARTIST)
        edit_add_artist(app, edit->input,
            edit->nb_artists > 0 ? ROLE_FEAT : ROLE_MAIN);
    if (buf && text_input(app, buf, code))
        edit_refresh_lists(app);
}

static int already_added(edit_t *edit, char const *name)
{
    for (int i = 0; i < edit->nb_artists; i++)
        if (strcasecmp(edit->artists[i].name, name) == 0)
            return 1;
    return 0;
}

/*
** an artist already known keeps its exact spelling
*/
void edit_add_artist(app_t *app, char const *name, role_t role)
{
    edit_t *edit = app->edit;
    artist_t *known = artist_find(app->lib, name);
    artist_role_t *slot = NULL;

    if (known)
        name = known->name;
    if (name[0] == '\0' || edit->nb_artists >= EDIT_ARTISTS
        || already_added(edit, name))
        return;
    slot = &edit->artists[edit->nb_artists];
    snprintf(slot->name, NAME_LEN, "%s", name);
    slot->role = role;
    edit->nb_artists++;
    edit->input[0] = '\0';
    edit_refresh_lists(app);
}

static void fill_suggest(app_t *app, edit_t *edit)
{
    edit->nb_suggest = 0;
    if (edit->input[0] == '\0')
        return;
    for (artist_t *a = app->lib->artists; a; a = a->next) {
        if (edit->nb_suggest >= EDIT_SUGGEST)
            return;
        if (str_contains(a->name, edit->input) && !already_added(edit,
                a->name) && artist_linked(app->lib, a->id)) {
            edit->suggest[edit->nb_suggest] = a;
            edit->nb_suggest++;
        }
    }
}

/*
** every song that can receive a merge: not itself, not an alias
*/
static void fill_results(app_t *app, edit_t *edit)
{
    edit->nb_results = 0;
    for (song_t *s = app->lib->songs; s; s = s->next) {
        if (edit->nb_results >= EDIT_RESULTS)
            return;
        if (s == edit->song || s->merged_into)
            continue;
        if (str_contains(song_title(s), edit->search)
            || (s->artists && str_contains(s->artists, edit->search))) {
            edit->results[edit->nb_results] = s;
            edit->nb_results++;
        }
    }
}

void edit_refresh_lists(app_t *app)
{
    fill_suggest(app, app->edit);
    fill_results(app, app->edit);
}
