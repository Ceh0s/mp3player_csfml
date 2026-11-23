/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** envelopes already computed, by fingerprint of the file (saved in
** envelopes.tsv): a file is only decoded once
*/

#include "mp3player.h"

envelope_t *envelope_find(library_t *lib, char const *hash)
{
    for (envelope_t *env = lib->envelopes; hash && env; env = env->next)
        if (strcmp(env->hash, hash) == 0)
            return env;
    return NULL;
}

/*
** data (nb values, NULL if the file could not be read) now belongs to
** the list
*/
envelope_t *envelope_add(library_t *lib, char const *hash,
    unsigned char *data, int nb)
{
    envelope_t *env = calloc(1, sizeof(envelope_t));

    if (!env || !hash) {
        free(env);
        free(data);
        return NULL;
    }
    env->hash = strdup(hash);
    env->data = data;
    env->nb = data ? nb : 0;
    env->next = lib->envelopes;
    lib->envelopes = env;
    return env;
}

void envelopes_free(library_t *lib)
{
    envelope_t *next = NULL;

    while (lib->envelopes) {
        next = lib->envelopes->next;
        free(lib->envelopes->hash);
        free(lib->envelopes->data);
        free(lib->envelopes);
        lib->envelopes = next;
    }
}

/*
** fingerprint of the file the present song is played from, NULL if none
*/
char const *song_hash(library_t *lib, song_t const *song)
{
    for (fprint_t *fp = lib->fprints; song->path && fp; fp = fp->next)
        if (fp->path && strcmp(fp->path, song->path) == 0)
            return fp->hash;
    return NULL;
}

/*
** envelope of the present song, computed if needed (NULL if its file is
** too long to be decoded, or cannot be read)
*/
envelope_t *song_envelope(library_t *lib, song_t const *song)
{
    char const *hash = song_hash(lib, song);
    envelope_t *env = envelope_find(lib, hash);
    unsigned char *data = NULL;
    int nb = 0;

    if (env || !hash)
        return env;
    if (song->duration_ms > 0 && song->duration_ms <= ENV_MAX_FILE_MS)
        data = envelope_compute(song->path, &nb);
    lib->envelopes_changed = 1;
    return envelope_add(lib, hash, data, nb);
}
