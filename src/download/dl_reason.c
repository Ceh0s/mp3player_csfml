/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** reason of a failed video, written by yt-dlp as "ERROR: [youtube]
** <id>: <reason>": the known reasons are said in French, the others are
** kept without the start; a lasting one puts the video in the
** unavailable ones (unavailable.c): only "not available", the old
** version of a song uploaded again (often Home, NCS...), the others are
** tried again at the next download
*/

#include "mp3player.h"

static const reason_t reasons[] = {
    {"This video is not available", "réupload : cette version n'est plus "
        "servie (titre republié)", 1},
    {"is private", "vidéo privée", 0},
    {"copyright", "retirée pour droits d'auteur", 0},
    {"Terms of Service", "retirée pour non-respect des règles de YouTube",
        0},
    {"account associated", "chaîne supprimée", 0},
    {"removed by the uploader", "supprimée par son auteur", 0},
    {"confirm your age", "réservée aux adultes (cookies nécessaires)", 0},
    {"not a bot", "YouTube demande une connexion (cookies)", 0},
    {NULL, NULL, 0}
};

static reason_t const *known(char const *detail)
{
    for (int i = 0; reasons[i].english; i++)
        if (strstr(detail, reasons[i].english))
            return &reasons[i];
    return NULL;
}

/*
** "ERROR: [youtube] abc: Video unavailable" -> "Video unavailable"
*/
static char const *without_start(char const *detail)
{
    char const *at = detail;

    if (strncmp(at, "ERROR: ", 7) == 0)
        at += 7;
    if (at[0] == '[' && strchr(at, ']') && strstr(strchr(at, ']'), ": "))
        return strstr(strchr(at, ']'), ": ") + 2;
    return at;
}

void dl_reason(char *out, size_t size, char const *detail)
{
    reason_t const *reason = known(detail ? detail : "");

    if (reason)
        snprintf(out, size, "%s", reason->french);
    else
        snprintf(out, size, "%s", without_start(detail ? detail : ""));
}

/*
** 1 if trying again would fail the same way
*/
int dl_reason_lasting(char const *detail)
{
    reason_t const *reason = known(detail ? detail : "");

    return reason && reason->lasting;
}
