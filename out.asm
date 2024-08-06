global _start
_start:
   mov rax,52
   push rax
   mov rax,101
   push rax
   push QWORD [rsp + 0]

    mov rax,60
    pop rdi
    syscall
