/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** one pair of the duplicates screen: how sure it is (likeness of the
** sound), the two songs (title, artists, duration, listening time,
** file), "Garder celui-ci" on each, and "Pas un doublon"
*/

#include "mp3player.h"

static const sfColor gray = {180, 180, 180, 255};
static const sfColor green = {120, 220, 120, 255};
static const sfColor orange = {255, 160, 60, 255};

static void verdict(app_t *app, dup_pair_t const *pair, float y)
{
    char line[128];
    sfColor const *color = &gray;

    if (pair->audio >= DUP_SURE)
        snprintf(line, sizeof(line), "Même enregistrement (son identique "
            "à %d %%)", pair->audio);
    else if (pair->audio >= 0)
        snprintf(line, sizeof(line), "Probable : son proche (%d %%), à "
            "écouter", pair->audio);
    else
        snprintf(line, sizeof(line), "Son non comparé (fichier illisible "
            "ou trop long)");
    if (pair->audio >= 0)
        color = pair->audio >= DUP_SURE ? &green : &orange;
    widget_text(app, line, (float [3]){130, y + 8, 22}, color);
}

static void song_line(app_t *app, song_t *song, float y)
{
    char line[NAME_LEN * 2 + 8];
    char cut[64 + 4];
    char time[32];
    char listen[32];
    char file[40 + 4];

    snprintf(line, sizeof(line), "%s%s%s", song_title(song),
        song->artists ? " - " : "", song->artists ? song->artists : "");
    utf8_cut(cut, line, 64);
    widget_text(app, cut, (float [3]){150, y + 10, 22}, &sfWhite);
    format_time(time, sizeof(time), song->duration_ms > 0 ?
        song->duration_ms : 0);
    format_time(listen, sizeof(listen), song_total_listen(app->lib, song));
    utf8_cut(file, song->file ? song->file : "", 40);
    snprintf(line, sizeof(line), "%s   écouté %s   %s", time, listen, file);
    widget_text(app, line, (float [3]){900, y + 12, 20}, &gray);
}

/*
** keep is kept, other is merged into it (its file removed first if
** asked: once merged, its files belong to keep)
*/
static void keep_song(app_t *app, song_t *keep, song_t *other)
{
    if (app->dupscene->remove)
        song_delete_files(app, other);
    song_merge(app, other, keep);
}

static void not_duplicate(app_t *app, dup_pair_t *pair)
{
    not_dup_add(app->lib, pair->a->id, pair->b->id);
    db_save_not_dups(app->lib);
    pair->a = NULL;
}

/*
** 0 : nothing clicked, 1 : not a duplicate, 2 : keep a, 3 : keep b
*/
static int choice(app_t *app, float y)
{
    float no[4] = {1540, y, 240, 44};

    widget_help(app, no, "Cette paire ne sera plus jamais proposée");
    if (widget_button(app, no, "Pas un doublon"))
        return 1;
    if (widget_button(app, (float [4]){1540, y + 48, 240, 44},
            "Garder celui-ci"))
        return 2;
    if (widget_button(app, (float [4]){1540, y + 98, 240, 44},
            "Garder celui-ci"))
        return 3;
    return 0;
}

/*
** returns 1 if a choice was made (the pairs changed)
*/
int dup_block(app_t *app, int i, float y)
{
    dup_pair_t *pair = &app->dupscene->list.pairs[i];
    int chosen = 0;

    verdict(app, pair, y);
    song_line(app, pair->a, y + 48);
    song_line(app, pair->b, y + 98);
    chosen = choice(app, y);
    if (chosen == 1)
        not_duplicate(app, pair);
    if (chosen == 2)
        keep_song(app, pair->a, pair->b);
    if (chosen == 3)
        keep_song(app, pair->b, pair->a);
    if (chosen)
        dupscene_clean(app);
    return chosen != 0;
}
