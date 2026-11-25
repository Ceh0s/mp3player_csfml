/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** title suggestions of the edit screen (see src/library/suggest): a
** line under the title on the main page, and a page listing each one
** with "Appliquer" (on the working copy, saved with "Enregistrer"),
** "Cacher" (shown again with "Afficher les cachées") and "Supprimer"
** (never proposed again for this song)
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

/*
** the suggestion changes the working copy (title and artists)
*/
static void apply(edit_t *edit, suggest_t const *sug)
{
    snprintf(edit->title, NAME_LEN, "%s", sug->title);
    suggest_apply_roles(edit->artists, &edit->nb_artists, sug);
}

/*
** the suggestions of the title being edited, hidden ones too if asked
*/
static int shown(app_t *app, edit_t *edit, suggest_list_t *list)
{
    int nb = 0;
    int state = 0;

    list->roles = edit->artists;
    list->nb_roles = edit->nb_artists;
    suggest_list(app->lib, edit->song, edit->title, list);
    for (int i = 0; i < list->nb; i++) {
        state = suggest_state(app->lib, edit->song->id, &list->items[i]);
        if (state == SUG_DELETED)
            continue;
        if (state == SUG_HIDDEN && !edit->show_hidden)
            continue;
        list->items[nb] = list->items[i];
        nb++;
    }
    list->nb = nb;
    return nb;
}

/*
** "Retirer « [HD] »", with the artist it adds; the spaces and the
** quotes have their own words (what they remove is not readable)
*/
static void describe(char *out, suggest_t const *sug)
{
    char const *const roles[NB_ROLES] = {"artiste", "featuring",
        "remixeur"};
    char removed[64];
    char const *start = sug->removed;

    while (*start == ' ')
        start++;
    utf8_cut(removed, start, 40);
    snprintf(out, NAME_LEN * 2, "Retirer « %s »", removed);
    if (sug->artist[0] != '\0')
        snprintf(out, NAME_LEN * 2, "Retirer « %s », %s : %s", removed,
            roles[sug->role], sug->artist);
    if (sug->rule == SUG_FEAT || sug->rule == SUG_REMIX)
        snprintf(out, NAME_LEN * 2, "Créditer %s en %s", sug->artist,
            roles[sug->role]);
    if (suggest_is_guess(sug->rule))
        snprintf(out, NAME_LEN * 2, "Supposition : artiste « %s »",
            sug->artist);
    if (sug->rule == SUG_SPACES)
        snprintf(out, NAME_LEN * 2, "Retirer les espaces en trop");
    if (sug->rule == SUG_QUOTES)
        snprintf(out, NAME_LEN * 2, "Retirer les guillemets autour du titre");
}

/*
** index of the first suggestion shown that is not a guess, -1 if none
*/
static int first_sure(app_t *app, edit_t *edit, suggest_list_t *list)
{
    int nb = shown(app, edit, list);

    for (int i = 0; i < nb; i++)
        if (!suggest_is_guess(list->items[i].rule))
            return i;
    return -1;
}

/*
** main page, under the title: the first suggestion, the page of all of
** them, and "Tout appliquer" (not the guesses)
*/
void edit_suggest_line(app_t *app, edit_t *edit)
{
    suggest_list_t list;
    float line[4] = {130, 335, 830, 48};
    char label[NAME_LEN * 2 + 32];
    char text[NAME_LEN * 2];

    if (shown(app, edit, &list) == 0)
        return;
    describe(text, &list.items[0]);
    snprintf(label, sizeof(label), "%d suggestion(s) : %s", list.nb, text);
    if (widget_button(app, line, label))
        edit->mode = EDIT_TITLES;
    if (!widget_button(app, (float [4]){975, 335, 255, 48},
            "Tout appliquer"))
        return;
    for (int done = 0; done < SUGGEST_MAX && first_sure(app, edit, &list)
        >= 0; done++)
        apply(edit, &list.items[first_sure(app, edit, &list)]);
}

static void row_text(app_t *app, suggest_t const *sug, float y,
    int hidden)
{
    char text[NAME_LEN * 2];

    describe(text, sug);
    widget_text(app, text, (float [3]){130, y, 24}, hidden ? &gray
        : &sfWhite);
    utf8_cut(text, sug->title, 80);
    widget_text(app, text, (float [3]){150, y + 34, 22}, &gray);
}

/*
** returns 1 if the list changed (the loop of the page stops there)
*/
static int row(app_t *app, edit_t *edit, suggest_t const *sug, float y)
{
    int hidden = suggest_state(app->lib, edit->song->id, sug) == SUG_HIDDEN;

    row_text(app, sug, y, hidden);
    if (widget_button(app, (float [4]){1240, y + 4, 180, 50}, "Appliquer")) {
        apply(edit, sug);
        return 1;
    }
    if (widget_button(app, (float [4]){1430, y + 4, 160, 50},
            hidden ? "Montrer" : "Cacher")) {
        suggest_set_state(app->lib, edit->song->id, sug,
            hidden ? SUG_VISIBLE : SUG_HIDDEN);
        return 1;
    }
    if (widget_button(app, (float [4]){1600, y + 4, 180, 50}, "Supprimer")) {
        suggest_set_state(app->lib, edit->song->id, sug, SUG_DELETED);
        return 1;
    }
    return 0;
}

static void titles_buttons(app_t *app, edit_t *edit)
{
    float y = 840 + app->extra_h;

    if (widget_button(app, (float [4]){130, y, 260, 56}, "Retour"))
        edit->mode = EDIT_MAIN;
    if (widget_button(app, (float [4]){410, y, 420, 56}, edit->show_hidden
            ? "Masquer les cachées" : "Afficher les cachées"))
        edit->show_hidden = !edit->show_hidden;
}

/*
** each suggestion is made on the title left by the ones applied before
*/
void edit_titles(app_t *app)
{
    edit_t *edit = app->edit;
    suggest_list_t list;
    int rows = (780 + app->extra_h - 300) / 80;

    widget_text(app, "Suggestions pour le titre", (float [3]){130, 125, 40},
        &sfWhite);
    widget_text(app, edit->title, (float [3]){130, 190, 26}, &gray);
    if (shown(app, edit, &list) == 0)
        widget_text(app, "Aucune suggestion.", (float [3]){130, 260, 26},
            &sfWhite);
    for (int i = 0; i < list.nb && i < rows; i++)
        if (row(app, edit, &list.items[i], 260 + i * 80))
            break;
    titles_buttons(app, edit);
}
