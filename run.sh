#!/bin/bash
nasm -felf64 build/out.asm
ld -o out build/out.o
./out
echo $?