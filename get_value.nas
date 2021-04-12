
;----------------------------------------------------------------------------------------            

default rel

global list_get_value_by_index

global check

extern hash_64

extern str_cmp


section .text


;----------------------------------------------------------------------------------------            
;Destroylist:  rax r10 r9

list_get_value_by_index:   
         
            mov rax, rsi
            add rax, rsi
            add rax, rsi
            shl rax, 3

            mov r10, [rdi]
            add r10, rax
            
            mov rax, [r10]
            mov r9, [r10 + 8]
            
            mov [rdx], rax
            mov [rdx + 8], r9

            ; pop r12

            xor rax, rax

            ret  

;;-------------------
get_key_by_index:   
         
            mov rax, rsi
            add rax, rsi
            add rax, rsi
            shl rax, 3

            mov r10, [rdi]
            add r10, rax
            
            mov rax, [r10]
            
            ret  

;;-------------------

get_value_by_index:   
         
            mov rax, rsi
            add rax, rsi
            add rax, rsi
            shl rax, 3

            mov r10, [rdi]
            add r10, rax
            
            mov rax, [r10 + 8]

            ret  



;struct Hash_table{
;    List** data;
;
;    long long size;
;    long long bucket_count;
;
;    size_t* sizes;
;
;    double load_factor;
;
;};




;rdi rsi rdx            (rbx, rsp, rbp, r12-r15
;bool get(Hash_table* ths, const char* key, const char** result){
;
;    size_t index = hash_64(key)%(ths->bucket_count); rdx
;    
;    List* ptr = ths->data[index]; 
;
;    if(!ptr){
;        return false;
;    }
;
;    size_t count = ptr->size;
;
;
;    Pair curr = {};
;
;    for(size_t i = 1; i <= count; i++){
;        list_get_value_by_index(ptr, i, &curr);
;
;
;        if(strcmp(key, curr.key) == 0){
;            *result = curr.value;
;    
;            return true;
;        }
;    }
;
;    return false;
;}





check:
            push rbx        ;save rbx

            mov r9, rsi     ;save input in registers    
            mov r11, rdi    ;save input in registers
            mov r8, rdi     ;save input in registers

            mov rdi, rsi
            call hash_64    ;hash input line
            xor rdx, rdx
            mov rbx, [r8 + 16]

            div rbx

                            ;RDX size_t index = hash_64(key)%(ths->bucket_count);
            ; mov rax, rdx   

            mov r8, [r8]    ;
            shl rdx, 3
            add r8, rdx

            mov r8, [r8]

            mov rax, r8     ;r8 = ths->data[index]

            cmp r8, 0
            jne skip_exit    ;    if(!ptr){
                pop rbx      ;        return false;
                             ;    }
                mov rax, 0
              
                ret
skip_exit:

            mov rcx, [r8 + 8]
            xor rbx, rbx
            inc rbx
                                ;for(size_t i = 1; i <= count; i++){
begin_loop:
            mov rdi, r8 
            mov rsi, rbx
            call get_key_by_index

            mov rdi, rax
            mov rsi, r9
            call str_cmp

            cmp rax, 0
            jne skip
 
              mov rdi, r8 
              mov rsi, rbx
              
              call get_value_by_index
              
              pop rbx 
              ret


skip:
            inc rbx
            cmp rbx, rcx
            jne begin_loop

            pop rbx

            mov rax, 0

            ret


