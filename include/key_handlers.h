#ifndef KEY_HANDLERS_H
#define KEY_HANDLERS_H

#include "life_drawer.h"


typedef struct
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool shift;
    bool control;
    bool alt;
}
control_keys;

extern control_keys pressed_keys;


// Handler for keydown event
void keydown_handler(SDL_Keycode key, volatile bool* pause, volatile direction* movement, volatile bool* move);

// Handler for keyup event
void keyup_handler(SDL_Keycode key, volatile bool* pause, volatile direction* movement,volatile bool* move);

#endif
