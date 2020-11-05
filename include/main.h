#ifndef MAIN_H
#define MAIN_H

#include "io_threader.h"
#include "key_handler.h"


extern uint32_t step_delay;  // minimum is 20
extern io_threader threader;

// Main
int main(void);

#endif
