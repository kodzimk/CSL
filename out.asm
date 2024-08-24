section .bss
 stringBuffer resb 100
stringBufferPos resb 8

section .data
text       times 255 db 0
textSize   equ $ - text
newLineMsg dw   0xA, 0xD
newLineLen equ $ - newLineMsg
temp dq  '',0xA,0xD

section .text
global _start
_start:
   mov rax,1
   push rax
   mov rax,'K'
   push rax
   mov rax,'a'
   push rax
   mov rax,'i'
   push rax
   mov rax,'s'
   push rax
   mov rax,'a'
   push rax
   mov rax,'r'
   push rax
    mov [temp],qword 'Kaisar'
    mov rax,1
    mov rsi,temp
    mov rdi,1
    mov rdx,6
    syscall
    
    mov edx,newLineLen
    mov ecx,newLineMsg
    mov ebx,1
    mov eax,4
    int 0x80
    
   mov rax,1
   push rax
    pop rdi
    jmp exit
exit:
    mov rax, 60
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
