section .bss
 stringBuffer resb 100
stringBufferPos resb 8

section .data
symbol db 's'
s db ' '

section .text
global _start
_start:
   mov rax,10
   push rax
   mov rax,100
   push rax
    pop rax
    call _printnumberRAX
    call euklid
   push QWORD [rsp + 0]

    mov rax,60
    pop rdi
    syscall

_printnumberRAX:
mov rcx, stringBuffer
mov rbx, 10
mov[rcx], rbx
inc rcx
mov[stringBufferPos], rcx
_printnumberRAXLoop :
mov rdx, 0
mov rbx, 10
div rbx
push rax
add rdx, 48
mov rcx, [stringBufferPos]
mov[rcx], dl
inc rcx
mov[stringBufferPos], rcx
pop rax;
cmp rax, 0
jne _printnumberRAXLoop
_printnumberRAXLoop2 :
mov rcx, [stringBufferPos]
mov rax, 1
mov rdi, 1
mov rsi, rcx
mov rdx, 1
syscall
mov rcx, [stringBufferPos]
dec rcx
mov[stringBufferPos], rcx
cmp rcx, stringBuffer
jge _printnumberRAXLoop2
ret
euklid :
cmp bx, cx
je finish
jg again
xchg bx, cx
again :
mov ax, bx
mov bx, cx
xor dx, dx
div cx
cmp dx, 0
je finish
mov bx, cx
mov cx, dx
jmp again
finish :
ret
