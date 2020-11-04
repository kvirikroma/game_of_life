#ifndef MAIN_H
#define MAIN_H

#include "io_threader.h"
#include "key_handler.h"


extern uint32_t step_delay;  // minimum is 20
extern bool run;
extern bool pause;
extern bool lmb_pressed;
extern bool rmb_pressed;
extern uint8_t speed;

extern bool moved_once;
extern direction movement;
extern bool move;
extern key_handler keyhandler;
extern io_threader threader;

// Main
int main(void);

// Apply scheduled movement actions
void apply_movement(bool lock_drawer);

#endif
