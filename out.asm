section .bss
 stringBuffer resb 100
stringBufferPos resb 8

section .data
text       times 255 db 0
textSize   equ $ - text
newLineMsg db 0xA, 0xD
newLineLen equ $ - newLineMsg
temp db 'a',0xA,0xD

section .text
global _start
_start:
   mov rax,1
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
   mov rax,19
   push rax
   push QWORD [rsp + 0]

    pop rax
    call _printnumberRAX
   mov rax,9
   push rax
   push QWORD [rsp + 0]

    pop rax
    call _printnumberRAX
   push QWORD [rsp + 56]

   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set0
    
    mov rsi, 0
carry_set0:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label0
 
   push QWORD [rsp + 56]

   mov rax,2
   push rax
    pop rbx
    pop rax
    add rax, rbx
   push rax
   push QWORD [rsp + 0]

    pop rax
    call _printnumberRAX
    pop rax
label0:
  
   mov rax,' '
   push rax
    mov rax,'f'
   push rax
   push QWORD [rsp + 0]

    pop rax
    mov [temp],rax
    mov rax,1
    mov rsi,temp
    mov rdi,1
    mov rdx,1
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
