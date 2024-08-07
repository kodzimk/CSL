global _start
_start:
   mov rax,5
   push rax
   mov rax,5
   push rax
   mov rax,2
   push rax
   mov rax,15
   push rax
    pop rax
    pop rbx
    mul rbx
   push rax
    pop rax
    pop rbx
    div rbx
   push rax
   mov rax,10
   push rax
    pop rax
    pop rbx
    add rax, rbx
   push rax
   push QWORD [rsp + 0]

    mov rax,60
    pop rdi
    syscall
