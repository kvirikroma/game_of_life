%include "array2d_include_common.asm"

struc bit_array2d
    .x_size     resd 1              ; array x axis size
    .y_size     resd 1              ; array y axis size
    .container  resb ARRAY2D_SIZE   ; container that serves data access
    .data       resq 0              ; begin of array data
endstruc

BIT_ARRAY2D_SIZE equ (8 + ARRAY2D_SIZE)
