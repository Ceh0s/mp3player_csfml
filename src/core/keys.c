/*
** EPITECH PROJECT, 2025
** mp3player
** File description:
** key repeat: on, so that a held backspace keeps erasing, but a held
** key must not repeat an action (Enter voting twice, Space toggling
** the music again and again); held keys are followed here
*/

#include "mp3player.h"

/*
** the keys whose repeat is useful: erasing and moving
*/
static int repeatable(sfKeyCode code)
{
    return code == sfKeyBackspace || code == sfKeyLeft
        || code == sfKeyRight || code == sfKeyUp || code == sfKeyDown;
}

/*
** a control character (Enter = 13, Tab = 9...) follows the KeyPressed
** of its key: it is dropped with it, backspace (8) is kept
*/
static int text_repeated(app_t *app, sfEvent const *event)
{
    sfUint32 code = event->text.unicode;

    return app->key_repeat && code < 32 && code != 8;
}

/*
** returns 1 if the event is the repeat of a held key, to ignore
*/
int key_repeated(app_t *app, sfEvent const *event)
{
    sfKeyCode code = sfKeyUnknown;

    if (event->type == sfEvtLostFocus)
        memset(app->held, 0, sizeof(app->held));
    if (event->type == sfEvtTextEntered)
        return text_repeated(app, event);
    if (event->type != sfEvtKeyPressed && event->type != sfEvtKeyReleased)
        return 0;
    code = event->key.code;
    if (code < 0 || code >= sfKeyCount)
        return 0;
    if (event->type == sfEvtKeyReleased) {
        app->held[code] = 0;
        return 0;
    }
    app->key_repeat = app->held[code];
    app->held[code] = 1;
    return app->key_repeat && !repeatable(code);
}
