section .data
symbol db 's'
s db 's'

section .text
global _start
_start:
    mov rax,[symbol]
    mov [s],rax
    mov [symbol],rax
   mov rax,1
   push rax
    mov [symbol], dword 'a'
    mov edx,1
    mov ecx,s
    mov ebx,1
    mov eax,4
    int 0x80
   push QWORD [rsp + 0]

    mov rax,60
    pop rdi
    syscall
