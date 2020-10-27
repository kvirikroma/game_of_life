#include <stdio.h>
#include <stdlib.h>

#include "include/bit_array2d.h"


void exit_on_syntax_error(void)
{
    fprintf(stderr, "%s", "Invalid command line parameters detected!\n");
    exit(1);
}


void* check_pointer_after_malloc(void* ptr)
{
    if (!ptr)
    {
        fprintf(stderr, "%s", "An error occurred while allocating memory! Exiting\n");
        exit(1);
    }
    return ptr;
}


int main(int argc, char** argv)
{
    return 0;
}
