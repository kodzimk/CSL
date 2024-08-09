section .data
symbol db 's'

section .text
global _start
_start:
    mov rax,[symbol]
    mov [symbol], dword '5'
    jmp printf

    mov [symbol],rax
   jmp printf

   mov rax,1
   push rax
   push QWORD [rsp + 0]

    mov rax,60
    pop rdi
    syscall

printf:
    mov edx,1
    mov ecx,symbol
    mov ebx,1
    mov eax,4
    int 0x80
