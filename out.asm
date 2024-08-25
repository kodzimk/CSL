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
   mov rax,10
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
   mov rax,0
   push rax
   push QWORD [rsp + 88]

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
 
   push QWORD [rsp + 88]

   push QWORD [rsp + 96]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 96]

    pop rax
   inc rax
   push rax
  
label0:
   push QWORD [rsp + 8]

   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set1
    
    mov rsi, 0
carry_set1:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label1
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label1:
   push QWORD [rsp + 8]

   mov rax,0
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
    cmp rax,0
    je label2
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label2:
   push QWORD [rsp + 8]

   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set3
    
    mov rsi, 0
carry_set3:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label3
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label3:
   push QWORD [rsp + 8]

   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set4
    
    mov rsi, 0
carry_set4:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label4
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label4:
   push QWORD [rsp + 8]

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
    je label5
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label5:
   push QWORD [rsp + 8]

   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set6
    
    mov rsi, 0
carry_set6:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label6
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label6:
   push QWORD [rsp + 8]

   mov rax,0
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
    je label7
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label7:
   push QWORD [rsp + 8]

   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set8
    
    mov rsi, 0
carry_set8:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label8
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label8:
   push QWORD [rsp + 8]

   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set9
    
    mov rsi, 0
carry_set9:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label9
 
   push QWORD [rsp + 8]

   push QWORD [rsp + 16]

    pop rax
   dec rax
   push rax
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label9:
   push QWORD [rsp + 8]

   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set10
    
    mov rsi, 0
carry_set10:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label10
 
label10:
  
   mov rax,10
   push rax
   push QWORD [rsp + 16]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set11
    
    mov rsi, 0
carry_set11:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label11
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,0
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set12
    
    mov rsi, 0
carry_set12:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label12
 
   push QWORD [rsp + 248]

    pop rax
    call _printnumberRAX
label12:
  
   push QWORD [rsp + 24]

    pop rax
   inc rax
   push rax
  
label11:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set13
    
    mov rsi, 0
carry_set13:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label13
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,1
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set14
    
    mov rsi, 0
carry_set14:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label14
 
label14:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label13:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set15
    
    mov rsi, 0
carry_set15:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label15
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,0
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set16
    
    mov rsi, 0
carry_set16:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label16
 
   push QWORD [rsp + 248]

    pop rax
    call _printnumberRAX
label16:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label15:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set17
    
    mov rsi, 0
carry_set17:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label17
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,1
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set18
    
    mov rsi, 0
carry_set18:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label18
 
label18:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label17:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set19
    
    mov rsi, 0
carry_set19:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label19
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,0
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set20
    
    mov rsi, 0
carry_set20:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label20
 
   push QWORD [rsp + 248]

    pop rax
    call _printnumberRAX
label20:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label19:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set21
    
    mov rsi, 0
carry_set21:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label21
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,1
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set22
    
    mov rsi, 0
carry_set22:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label22
 
label22:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label21:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set23
    
    mov rsi, 0
carry_set23:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label23
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,0
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set24
    
    mov rsi, 0
carry_set24:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label24
 
   push QWORD [rsp + 248]

    pop rax
    call _printnumberRAX
label24:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label23:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set25
    
    mov rsi, 0
carry_set25:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label25
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,1
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set26
    
    mov rsi, 0
carry_set26:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label26
 
label26:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label25:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set27
    
    mov rsi, 0
carry_set27:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label27
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,0
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set28
    
    mov rsi, 0
carry_set28:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label28
 
   push QWORD [rsp + 248]

    pop rax
    call _printnumberRAX
label28:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label27:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set29
    
    mov rsi, 0
carry_set29:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label29
 
   push QWORD [rsp + 232]

   mov rax,2
   push rax
    mov rax,1
   push rax
   mov rax,0
   push rax
    pop rbx
    pop rax
    mov rsi, 1
    cmp rax, rbx
    je carry_set30
    
    mov rsi, 0
carry_set30:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label30
 
label30:
  
   push QWORD [rsp + 16]

    pop rax
   inc rax
   push rax
  
label29:
   mov rax,10
   push rax
   push QWORD [rsp + 8]

    pop rbx
    pop rax
    mov rsi, 1
    cmp rbx, rax
    jc carry_set31
    
    mov rsi, 0
carry_set31:
    
mov rax,rsi
   push rax
    pop rax
    cmp rax,0
    je label31
 
label31:
  
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
