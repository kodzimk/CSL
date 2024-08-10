section .data
symbol db 's'
s db 'b'

section .text
global _start
_start:
    mov rax,[s]
    mov [symbol],rax
    mov [s],rax
    mov edx,1
    mov ecx,symbol
    mov ebx,1
    mov eax,4
    int 0x80
   mov rax,1
   push rax
   push QWORD [rsp + 0]

    mov rax,60
    pop rdi
    syscall
