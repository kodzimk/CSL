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
   mov rax,10
   push rax
    mov edx,1
    mov ecx,symbol
    mov ebx,1
    mov eax,4
    int 0x80
    
    mov edx,newLineLen
    mov ecx,newLineMsg
    mov ebx,1
    mov eax,4
    int 0x80
    
    mov [temp],dword 'S'
    mov ecx,temp
    mov edx,1
    mov ebx,1
    mov eax,4
    int 0x80
    
   mov rax,2
   push rax
   mov rax,5
   push rax
    pop rax
    pop rbx
    mul rbx
   push rax
   mov rax,1
   push rax
    pop rax
    pop rbx
    add rax, rbx
   push rax
    pop rax
    call _printnumberRAX
    mov edx,newLineLen
    mov ecx,newLineMsg
    mov ebx,1
    mov eax,4
    int 0x80
    
   mov rax,100
   push rax
    pop rax
    call _printnumberRAX
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
