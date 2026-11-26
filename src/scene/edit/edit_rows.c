/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** one row of the artists list of the edit screen:
** name (opens the artist page), listening time, role, remove button
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};

/*
** a name written as a list ("A, B") can be split into its parts (done
** when the song is saved)
*/
static int split_button(app_t *app, edit_t *edit, int i, float y)
{
    float rect[4] = {470, y, 300, 46};

    if (!strchr(edit->artists[i].name, ','))
        return 0;
    widget_help(app, rect, "Sinon le nom est gardé entier");
    if (widget_button(app, rect, "Séparer"))
        roles_explode(edit->artists, &edit->nb_artists, i);
    return 1;
}

static void name_and_time(app_t *app, edit_t *edit, int i, float y)
{
    artist_t *known = artist_find(app->lib, edit->artists[i].name);
    char time[16];
    char label[64];

    format_time(time, sizeof(time),
        known ? artist_listen_ms(app->lib, known->id) : 0);
    snprintf(label, sizeof(label), "%s d'écoute", time);
    if (!split_button(app, edit, i, y))
        widget_text(app, label, (float [3]){470, y + 12, 20}, &gray);
    if (widget_button(app, (float [4]){130, y, 320, 46},
            edit->artists[i].name)) {
        edit->artist = known;
        edit->mode = EDIT_ARTIST;
        scroll_jump(&edit->titles, 0);
        edit->image.msg[0] = '\0';
    }
}

void edit_artist_row(app_t *app, edit_t *edit, int i)
{
    char const *roles[NB_ROLES] = {"principal", "feat.", "remix"};
    float y = 440 + i * 54;

    name_and_time(app, edit, i, y);
    if (widget_button(app, (float [4]){800, y, 170, 46},
            roles[edit->artists[i].role]))
        edit->artists[i].role = (edit->artists[i].role + 1) % NB_ROLES;
    if (!widget_button(app, (float [4]){990, y, 170, 46}, "Retirer"))
        return;
    edit->nb_artists--;
    memmove(&edit->artists[i], &edit->artists[i + 1],
        sizeof(artist_role_t) * (edit->nb_artists - i));
}
