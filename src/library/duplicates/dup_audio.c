/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** sound envelope of a file: loudness every ENV_FRAME_MS, read by CSFML
** (the whole file is decoded, then freed at once: one file at a time).
** Two envelopes are compared by correlation, the second one being moved
** a little (an intro added to a clip): close to 1, same recording.
*/

#include "mp3player.h"
#include <math.h>

/*
** loudness of n samples, from -80 dB (0) to 0 dB (255)
*/
static unsigned char frame_level(sfInt16 const *samples, unsigned int n)
{
    double sum = 0;
    double db = 0;

    for (unsigned int i = 0; i < n; i++)
        sum += (double)samples[i] * samples[i];
    sum = sqrt(sum / n);
    if (sum < 1)
        return 0;
    db = 20 * log10(sum / 32768.0);
    if (db < -80)
        return 0;
    return (unsigned char)((db + 80) * 255 / 80);
}

/*
** one level per ENV_FRAME_MS of the decoded samples
*/
static unsigned char *levels(sfSoundBuffer *buf, int *nb)
{
    sfInt16 const *samples = sfSoundBuffer_getSamples(buf);
    unsigned int step = sfSoundBuffer_getSampleRate(buf) * ENV_FRAME_MS
        / 1000 * sfSoundBuffer_getChannelCount(buf);
    unsigned char *env = NULL;

    *nb = step && samples ? sfSoundBuffer_getSampleCount(buf) / step : 0;
    *nb = *nb > ENV_MAX ? ENV_MAX : *nb;
    env = *nb > 0 ? malloc(*nb) : NULL;
    for (int i = 0; env && i < *nb; i++)
        env[i] = frame_level(samples + (long)i * step, step);
    *nb = env ? *nb : 0;
    return env;
}

/*
** returns the envelope (nb values, at most ENV_MAX), NULL if the file
** cannot be read
*/
unsigned char *envelope_compute(char const *path, int *nb)
{
    sfSoundBuffer *buf = sfSoundBuffer_createFromFile(path);
    unsigned char *env = NULL;

    *nb = 0;
    if (!buf)
        return NULL;
    env = levels(buf, nb);
    sfSoundBuffer_destroy(buf);
    return env;
}

/*
** sums of the changes of loudness da, db: of da, of db, of da * db, of
** da * da and of db * db
*/
static void add_sums(double *sums, double da, double db)
{
    sums[0] += da;
    sums[1] += db;
    sums[2] += da * db;
    sums[3] += da * da;
    sums[4] += db * db;
}

/*
** Pearson correlation of the changes of loudness of n values (0 if one
** of them does not move): the changes (a beat, a drop) tell two songs
** apart better than the loudness itself, close in most songs
*/
static double correlation(unsigned char const *a, unsigned char const *b,
    int n)
{
    double sums[5] = {0, 0, 0, 0, 0};
    double var_a = 0;
    double var_b = 0;

    n--;
    for (int i = 0; i < n; i++)
        add_sums(sums, (double)a[i + 1] - a[i], (double)b[i + 1] - b[i]);
    var_a = sums[3] - sums[0] * sums[0] / n;
    var_b = sums[4] - sums[1] * sums[1] / n;
    if (var_a <= 0 || var_b <= 0)
        return 0;
    return (sums[2] - sums[0] * sums[1] / n) / sqrt(var_a * var_b);
}

/*
** likeness from 0 to 100, the best one with b moved by up to ENV_SHIFT
** values; -1 if one of them is too short to tell
*/
int envelope_compare(envelope_t const *a, envelope_t const *b)
{
    double best = -2;
    double value = 0;
    int start_a = 0;
    int start_b = 0;
    int n = 0;

    for (int off = -ENV_SHIFT; off <= ENV_SHIFT; off++) {
        start_a = off > 0 ? off : 0;
        start_b = off < 0 ? -off : 0;
        n = a->nb - start_a < b->nb - start_b ? a->nb - start_a
            : b->nb - start_b;
        if (n < ENV_MIN)
            continue;
        value = correlation(a->data + start_a, b->data + start_b, n);
        best = value > best ? value : best;
    }
    if (best < -1)
        return -1;
    return best > 0 ? (int)(best * 100) : 0;
}
