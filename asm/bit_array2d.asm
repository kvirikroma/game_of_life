%include "array2d_include.asm"

global bit_array2d_init     ; initializes array of needed size in memory
global bit_array2d_delete   ; deletes an bit_array2d* (param rdi - bit_array2d* to delete)
global bit_array2d_get_bit  ; returns value of bit by its coordinates
global bit_array2d_set_bit  ; sets bit value by its coordinates


segment .text
    bit_array2d_init:
        ; param rdi - size by X axis (in bits)
        ; param rsi - size by Y axis (in bits)
        ret

    bit_array2d_delete:
        ; param rdi - bit_array2d*
        call array2d_delete
        ret

    bit_array2d_get_bit:
        ; param rdi - coordinate by X axis (in bits)
        ; param rsi - coordinate by Y axis (in bits)
        ; returns 0 or 1 (bit value)
        ret

    bit_array2d_set_bit:
        ; param rdi - coordinate by X axis (in bits)
        ; param rsi - coordinate by Y axis (in bits)
        ; param rdx - value (0 or 1), every non-0 value will be accepted as 1
        ; returns 1 on success or 0 on error
        ret
