%ifndef bit_array2d_include_common
%define bit_array2d_include_common


struc bit_array2d
    .x_size     resd 1  ; array x axis size (in bits)
    .y_size     resd 1  ; array y axis size (in bits)
    .data       resq 0  ; begin of array data
endstruc

BIT_ARRAY2D_SIZE equ 8


%endif
