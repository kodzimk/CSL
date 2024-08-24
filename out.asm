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
   mov rax,'s'
   push rax
   mov rax,'s'
   push rax
   mov rax,'B'
   push rax
   mov rax,'B'
   push rax
   mov rax,315
   push rax
   mov rax,317
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set0
    
    mov rsi, 0
carry_set0:
    
mov rax,rsi
   push rax
   mov rax,67
   push rax
   mov rax,52
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set1
    
    mov rsi, 0
carry_set1:
    
mov rax,rsi
   push rax
   mov rax,52
   push rax
   mov rax,49
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set2
    
    mov rsi, 0
carry_set2:
    
mov rax,rsi
   push rax
    pop rax
    pop rbx
    mov rsi, 1
    or rax, rbx
    jne carry_set3
    
    mov rsi, 0
    carry_set3:
    
mov rax,rsi
   push rax
    pop rax
    pop rbx
    mov rsi, 1
    or rax, rbx
    jne carry_set4
    
    mov rsi, 0
    carry_set4:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label0
 
    mov [temp + 0], byte 'K'
    mov [temp + 1], byte 'a'
    mov [temp + 2], byte 'i'
    mov [temp + 3], byte 's'
    mov [temp + 4], byte 'a'
    mov [temp + 5], byte 'r'
    mov rax,1
    mov rsi,temp
    mov rdi,1
    mov rdx,6
    syscall
    
    pop rax
label0:
  
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
