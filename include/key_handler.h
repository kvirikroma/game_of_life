#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H

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

typedef struct
{
    control_keys pressed_keys;
    bool* pause;
    bool* move;
    direction* movement;
    uint8_t* speed;
}
key_handler;


// Init the key_handler
void key_handler_init(key_handler* self, bool* pause, direction* movement, bool* move, uint8_t* speed);

// Handler for keydown event
// Retyrns true if applies some actions
bool key_handler_down(key_handler* self, SDL_Keycode key);

// Handler for keyup event
// Retyrns true if applies some actions
bool key_handler_up(key_handler* self, SDL_Keycode key);

#endif
