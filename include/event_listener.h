#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "key_handler.h"


typedef struct
{
    key_handler keyhandler;
    SDL_Event event;
    bool run;
    bool pause;
    bool lmb_pressed;
    bool rmb_pressed;
    uint8_t speed;
    bool moved_once;
    direction movement;
    bool move;
}
event_listener;


void event_listener_init(event_listener* self);

void event_listener_listen(event_listener* self, void* threader);

void event_listener_apply_movement(event_listener* self, void* threader, bool lock_drawer);

#endif