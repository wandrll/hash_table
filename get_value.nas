
;----------------------------------------------------------------------------------------            

default rel

global hash_table_get

extern hash_64

extern str_cmp


section .text




hash_table_get:

            push r12        ;save r12
            mov r11, rbx    ;save rbx in registers


            mov r9, rsi     ;save input in registers    
            mov r12, rdi     ;save input in registers
            mov rdi, rsi

            call hash_64    ;hash input line
            
            xor rdx, rdx
            
            mov rbx, [r12 + 16]

            div rbx

                            ;RDX size_t index = hash_64(key)%(ths->bucket_count);
            mov r12, [r12]    
            shl rdx, 3
            add r12, rdx

            mov r12, [r12]

            mov rax, r12     ;r12 = ths->data[index]

            cmp r12, 0
            jne .skip_exit    ;    if(!ptr){
                mov rbx, r11      ;        return false;
                pop r12
                             ;    }
                mov rax, 0
              
                ret


                
.skip_exit:

            mov rbx, [r12 + 8]
                                ;for(size_t i = 1; i <= count; i++){

            mov r12, [r12]      ; r12 = ths->data[index]->data

.begin_loop:
            add r12, 24         ;mov in r12 address of ths->data[index]->data[i].key
            mov rdi, [r12]      ;mov in rdi key

            mov rsi, r9         
            call str_cmp        ;compare keys
            
            cmp rax, 0
            jne .skip           ;if we found exact key
  
              mov rax, [r12 + 8]
              
              mov rbx, r11
              pop r12 
              ret


.skip:
            dec rbx             ;if there is no element with unput key - return NULL
            cmp rbx, 0
            jne .begin_loop

            mov rbx, r11
            pop r12

            mov rax, 0

            ret



