/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** core of a title, to compare two uploads of the same song:
** "Madeon - You're On ft. Kyan (Official Video)" and "You're On" both
** give "you re on". What is before " - " is often the artist (the way an
** uploader writes it): it is given back to be compared with the artists.
*/

#include "mp3player.h"

/*
** the text after these marks is not part of the title
*/
static char const *const tails[] = {" | ", " \xef\xbd\x9c ", " feat. ",
    " feat ", " ft. ", " ft ", " featuring ", " Feat. ", " Ft. ",
    " FEAT. ", " FT. ", NULL};

static void cut_tails(char *title)
{
    char *found = NULL;

    for (int i = 0; tails[i]; i++) {
        found = strstr(title, tails[i]);
        if (found)
            *found = '\0';
    }
}

/*
** the parts between () or [] are removed (their version words were
** read on the whole title)
*/
static void remove_brackets(char *title)
{
    int depth = 0;
    int n = 0;

    for (int i = 0; title[i]; i++) {
        depth += title[i] == '(' || title[i] == '[';
        if (depth == 0) {
            title[n] = title[i];
            n++;
        }
        depth -= depth > 0 && (title[i] == ')' || title[i] == ']');
    }
    title[n] = '\0';
}

/*
** " - " or " – " (en dash), the first one
*/
static char const *find_dash(char const *title, int *len)
{
    char const *dash = strstr(title, " - ");
    char const *en = strstr(title, " \xe2\x80\x93 ");

    *len = 3;
    if (en && (!dash || en < dash)) {
        *len = 5;
        return en;
    }
    return dash;
}

/*
** core : clean core of the title (NAME_LEN bytes), prefix : the raw text
** before " - " ("" if none)
*/
void dup_core(char const *title, char *core, char *prefix)
{
    char raw[NAME_LEN];
    int len = 0;
    char const *dash = find_dash(title, &len);

    prefix[0] = '\0';
    if (dash && dash - title < NAME_LEN) {
        snprintf(prefix, dash - title + 1, "%s", title);
        title = dash + len;
    }
    snprintf(raw, NAME_LEN, "%s", title);
    cut_tails(raw);
    remove_brackets(raw);
    dup_clean(raw, core, NAME_LEN);
    dup_remove_noise(core);
}
