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
    mov rax,'Kaisar'
   push rax
    mov rax,'Kaisar2'
   push rax
   mov rax,'s'
   push rax
   mov rax,'s'
   push rax
   mov rax,'B'
   push rax
   mov rax,'B'
   push rax
   mov rax,317
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
    cmp rax, rbx
    jc carry_set2
    
    mov rsi, 0
carry_set2:
    
mov rax,rsi
   push rax
    pop rax
    pop rbx
    mov rsi, 1
    and rbx, rax
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
 
   mov rax,5
   push rax
   mov rax,'l'
   push rax
   mov rax,0
   push rax
   mov rax,0
   push rax
   mov rax,0
   push rax
   mov rax,0
   push rax
   mov rax,0
   push rax
    mov rax,'sasa'
   push rax
   mov rax,1
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set5
    
    mov rsi, 0
carry_set5:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label1
 
   mov rax,5
   push rax
   push QWORD [rsp + 0]

    pop rax
    call _printnumberRAX
    pop rax
label1:
  
    pop rax
    pop rax
    pop rax
    pop rax
    pop rax
    pop rax
    pop rax
    pop rax
label0:
  
   mov rax,5
   push rax
   push QWORD [rsp + 0]

    pop rax
    call _printnumberRAX
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
