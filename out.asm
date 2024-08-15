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
   mov rax,2
   push rax
   mov rax,1
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
   mov rax,1
   push rax
   mov rax,1
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    jc carry_set1
    
    mov rsi, 0
carry_set1:
    
mov rax,rsi
   push rax
    pop rax
    pop rbx
    mov rsi, 1
    and rbx, rax
    jne carry_set2
    
    mov rsi, 0
carry_set2:
    
mov rax,rsi
   push rax
   mov rax,0
   push rax
   mov rax,1
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set3
    
    mov rsi, 0
carry_set3:
    
mov rax,rsi
   push rax
   mov rax,1
   push rax
   mov rax,2
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jge carry_set4
    
    mov rsi, 0
carry_set4:
    
mov rax,rsi
   push rax
    pop rax
    pop rbx
    mov rsi, 1
    and rbx, rax
    jne carry_set5
    
    mov rsi, 0
carry_set5:
    
mov rax,rsi
   push rax
    pop rax
    pop rbx
    mov rsi, 1
    or rax, rbx
    jne carry_set6
    
    mov rsi, 0
    carry_set6:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label0
 
    jmp label1
label0:
   mov rax,2
   push rax
   mov rax,2
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set7
    
    mov rsi, 0
carry_set7:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label2
 
    jmp label3
label2:
   mov rax,2
   push rax
   push QWORD [rsp + 16]

    pop rax
    pop rbx
    mul rbx
   push rax
    add rsp, 0
label3:
  
label1:
  
   push QWORD [rsp + 8]

    pop rax
    call _printnumberRAX
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
