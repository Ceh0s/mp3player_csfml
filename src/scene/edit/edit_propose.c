/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** guesses the real artists from the ID3 title, only as a proposal:
** - "X - Title" when X is an artist already known (a " - " alone is
**   not enough: it can be part of the title)
** - "(feat. X)", "(ft. X)" : featuring, "(X Remix)" : remixer
*/

#include "mp3player.h"
#include <strings.h>

static void add_proposal(edit_t *edit, char const *name, int len,
    role_t role)
{
    artist_role_t *slot = &edit->proposal[edit->nb_proposal];

    while (len > 0 && *name == ' ') {
        name++;
        len--;
    }
    while (len > 0 && name[len - 1] == ' ')
        len--;
    if (len <= 0 || len >= NAME_LEN || edit->nb_proposal >= EDIT_ARTISTS)
        return;
    for (int i = 0; i < edit->nb_proposal; i++)
        if (strncasecmp(edit->proposal[i].name, name, len) == 0
            && edit->proposal[i].name[len] == '\0')
            return;
    snprintf(slot->name, len + 1, "%s", name);
    slot->role = role;
    edit->nb_proposal++;
}

/*
** removes start..end from the title, with the space before it
*/
static void cut(char *title, char *start, char *end)
{
    if (start > title && start[-1] == ' ')
        start--;
    memmove(start, end + 1, strlen(end + 1) + 1);
}

/*
** name becomes the only main artist, the featurings and remixers stay
*/
static void set_main(edit_t *edit, char const *name)
{
    int kept = 0;

    for (int i = 0; i < edit->nb_proposal; i++) {
        if (edit->proposal[i].role == ROLE_MAIN)
            continue;
        edit->proposal[kept] = edit->proposal[i];
        kept++;
    }
    if (kept >= EDIT_ARTISTS)
        kept = EDIT_ARTISTS - 1;
    memmove(&edit->proposal[1], &edit->proposal[0],
        sizeof(artist_role_t) * kept);
    snprintf(edit->proposal[0].name, NAME_LEN, "%s", name);
    edit->proposal[0].role = ROLE_MAIN;
    edit->nb_proposal = kept + 1;
}

/*
** "X - Title" with X a known artist: X becomes the only main artist
** (the uploader of the video is dropped)
*/
static int find_dash(app_t *app, edit_t *edit)
{
    char *title = edit->proposal_title;
    char *dash = strstr(title, " - ");
    char name[NAME_LEN];
    artist_t *artist = NULL;

    if (!dash || dash - title >= NAME_LEN)
        return 0;
    snprintf(name, dash - title + 1, "%s", title);
    artist = artist_find(app->lib, name);
    if (!artist)
        return 0;
    set_main(edit, artist->name);
    memmove(title, dash + 3, strlen(dash + 3) + 1);
    return 1;
}

static int find_marker(edit_t *edit, char const *marker, role_t role)
{
    char *start = strstr(edit->proposal_title, marker);
    char *end = start ? strchr(start, ')') : NULL;
    int len = strlen(marker);

    if (!end)
        return 0;
    add_proposal(edit, start + len, end - start - len, role);
    cut(edit->proposal_title, start, end);
    return 1;
}

static int find_remix(edit_t *edit)
{
    char *title = edit->proposal_title;
    char *end = strstr(title, " Remix)");
    char *start = NULL;

    if (!end)
        end = strstr(title, " remix)");
    if (!end)
        return 0;
    start = end;
    while (start > title && *start != '(')
        start--;
    if (*start != '(')
        return 0;
    add_proposal(edit, start + 1, end - start - 1, ROLE_REMIX);
    cut(title, start, end + 6);
    return 1;
}

static int differs(edit_t *edit)
{
    if (strcmp(edit->proposal_title, edit->title) != 0
        || edit->nb_proposal != edit->nb_artists)
        return 1;
    for (int i = 0; i < edit->nb_artists; i++)
        if (strcmp(edit->proposal[i].name, edit->artists[i].name) != 0
            || edit->proposal[i].role != edit->artists[i].role)
            return 1;
    return 0;
}

void edit_propose(app_t *app)
{
    edit_t *edit = app->edit;
    char const *markers[4] = {"(feat. ", "(ft. ", "(Feat. ", "(Ft. "};
    int found = 0;

    snprintf(edit->proposal_title, NAME_LEN, "%s", edit->song->src_title
        ? edit->song->src_title : edit->title);
    memcpy(edit->proposal, edit->artists, sizeof(edit->artists));
    edit->nb_proposal = edit->nb_artists;
    found += find_dash(app, edit);
    for (int i = 0; i < 4; i++)
        found += find_marker(edit, markers[i], ROLE_FEAT);
    found += find_remix(edit);
    edit->has_proposal = found > 0 && differs(edit);
}

void edit_apply_proposal(app_t *app)
{
    edit_t *edit = app->edit;

    snprintf(edit->title, NAME_LEN, "%s", edit->proposal_title);
    memcpy(edit->artists, edit->proposal, sizeof(edit->artists));
    edit->nb_artists = edit->nb_proposal;
    edit->has_proposal = 0;
}
