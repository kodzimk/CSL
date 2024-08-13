section .bss
 stringBuffer resb 100
stringBufferPos resb 8

section .data
newLineMsg db 0xA, 0xD
newLineLen equ $ - newLineMsg
temp db 'a',0xA,0xD
symbol db 's',0xA,0xD
s db ' ',0xA,0xD

section .text
global _start
_start:
   mov rax,2
   push rax
   mov rax,3
   push rax
   mov rax,4
   push rax
    pop rax
    pop rbx
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
 
   mov rax,3
   push rax
   push QWORD [rsp + 8]

    pop rax
    pop rbx
    add rax, rbx
   push rax
    add rsp, 0
label0:
  
   push QWORD [rsp + 0]

    pop rax
   inc rax
   push rax
   push QWORD [rsp + 0]

    pop rax
   inc rax
   push rax
   push QWORD [rsp + 0]

    pop rax
   inc rax
   push rax
   push QWORD [rsp + 0]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 0]

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
