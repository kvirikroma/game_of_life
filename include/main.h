#ifndef MAIN_H
#define MAIN_H

// Print error message to an stderr end exit
void exit_on_syntax_error(void);

// Check pointer after malloc and exit if it is null
void* check_pointer_after_malloc(void* ptr);

// Main
int main(int argc, char** argv);

#endif
