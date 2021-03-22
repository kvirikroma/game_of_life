#!/bin/bash

cd ./asm
mkdir -p ./compiler_symlinks
rm -rf ./compiler_symlinks
mkdir ./compiler_symlinks

for f in ./*.asm
do
    base_f="$(basename -- $f)"
    ln -s ../$f ./compiler_symlinks/$base_f
done

for f in ./include/*.asm
do
    base_f="$(basename -- $f)"
    ln -s ../$f ./compiler_symlinks/$base_f
done

cd ./compiler_symlinks/
for f in ./*.asm
do
    if \
        nasm -f elf64 $f -o ../${f::-4}.o &&\
        yasm -f elf64 -g dwarf2 $f -o ../${f::-4}.o
    then
        true
    else
        exit
    fi
done
cd ../

rm -rf ./compiler_symlinks
cd ../

if [[ $1 == 'release' ]]; then
    gcc -Wall -m64 -no-pie -o ./game_of_life ./*.c ./asm/*.o -Bstatic -lSDL2 -lm -lpthread -Ofast
else
    gcc -Wall -m64 -no-pie -gdwarf-2 -o ./game_of_life ./*.c ./asm/*.o -lSDL2 -lm -lpthread -Ofast
fi
#clang -Wall -no-pie -gdwarf-2 ./*.c ./asm/*.o -Bstatic -lSDL2 -lm -lpthread -m64 -Ofast -o ./game_of_life

rm -rf ./asm/*.o
rm -rf ./*.o
