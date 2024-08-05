#!/bin/bash
nasm -felf64 out.asm
ld -o out out.o
./out
echo $?