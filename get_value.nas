
;----------------------------------------------------------------------------------------            

default rel

global list_get_value_by_index

section .text


;----------------------------------------------------------------------------------------            
;Destroylist:  rax rcx r8

list_get_value_by_index:   
         
            mov rax, rsi
            add rax, rsi
            add rax, rsi
            shl rax, 3

            mov rcx, [rdi]
            add rcx, rax
            
            mov rax, [rcx]
            mov r8, [rcx + 8]
            
            mov [rdx], rax
            mov [rdx + 8], r8

            ; pop r12

            xor rax, rax

            ret  

;;-------------------
