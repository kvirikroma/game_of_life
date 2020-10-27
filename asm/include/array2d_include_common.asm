struc array2d
    .x_size     resd 1  ; array x axis size
    .y_size     resd 1  ; array y axis size
    .item_size  resw 1  ; size of each array item (in bytes)
    .data       resq 0  ; begin of array data
endstruc

ARRAY2D_SIZE equ 10
