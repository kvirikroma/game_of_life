#!/bin/bash

for f in ./asm/*
do
    if \
        nasm -f elf64 $f -o ${f::-4}.o &&\
        yasm -f elf64 -g dwarf2 $f -o ${f::-4}.o
    then
        true
    else
        exit
    fi
done

gcc -Wall -m64 -no-pie -gdwarf-2 -o ./filerm ./*.c ./asm/*.o

rm -rf ./asm/*.o
rm -rf ./*.o
