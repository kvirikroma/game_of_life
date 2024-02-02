#!/bin/bash

set -m

cd ./asm
mkdir -p ./compiler_symlinks
rm -rf ./compiler_symlinks
mkdir ./compiler_symlinks

# Ask for symlinks
for f in ./*.asm
do
    base_f="$(basename -- $f)"
    ln -s ../$f ./compiler_symlinks/$base_f &
done

for f in ./include/*.asm
do
    base_f="$(basename -- $f)"
    ln -s ../$f ./compiler_symlinks/$base_f &
done

# Wait for completion
for f in ./*.asm
do
    fg &> /dev/null
done

for f in ./include/*.asm
do
    fg &> /dev/null
done

cd ./compiler_symlinks/
# Compile asm
for f in ./*.asm
do
    if \
        nasm -f elf64 -F dwarf $f -o ../${f::-4}.o &
    then
        true
    else
        exit
    fi
done

# Wait for completion again
for f in ./*.asm
do
    fg &> /dev/null
done
cd ../

rm -rf ./compiler_symlinks
cd ../

if [[ $1 == 'release' ]]; then
    echo "building release"
    gcc -Wall -m64 -no-pie -o ./game_of_life ./*.c ./asm/*.o -Bstatic ./static_libs/libSDL2.a -lm -lpthread -Ofast
else
    gcc -Wall -m64 -no-pie -gdwarf-2 -o ./game_of_life ./*.c ./asm/*.o -lSDL2 -lm -lpthread -Ofast
fi
#clang -Wall -no-pie -gdwarf-2 ./*.c ./asm/*.o -Bstatic -lSDL2 -lm -lpthread -m64 -Ofast -o ./game_of_life

strip ./game_of_life
rm -rf ./asm/*.o
rm -rf ./*.o
