#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.hpp"
#include "hash_table.hpp"
#include <immintrin.h>

void constructor(Hash_table* ths){
    ths->data = (List**)calloc(1024, sizeof(List*));


    ths->load_factor = 0;
    ths->size = 0;
    ths->bucket_count = 1024;

}

static void free_buckets(Hash_table* ths){

    for(int i = 0; i < ths->bucket_count; i++){
        if(ths->data[i]){
            Delete_List(ths->data[i]);
        }
    }
}

void destructor(Hash_table* ths){
    free_buckets(ths);
    free(ths->data);
}


static inline void update_load_factor(Hash_table* ths){
    ths->load_factor = ((double)(ths->size))/((double)(ths->bucket_count));
}



static unsigned long long hash_64(const char* line){
    
    unsigned long long hash = 0;

    while (*((unsigned long long*)line)){
        hash = _mm_crc32_u64(hash, *((unsigned long long*)line));
        line+=8;
    }

    return hash;


}





static void resize(Hash_table* ths){
    List** new_data   = (List**)calloc(ths->bucket_count*2, sizeof(List*));

    size_t count = ths->bucket_count;
    ths->bucket_count = ths->bucket_count*2;

    for(size_t i = 0; i < count; i++){
        
        if(ths->data[i]){

            const size_t curr_count = ths->data[i]->size;
            
            for(int j = 0; j < curr_count; j++){

                Pair curr = {};
                list_pop_back(ths->data[i], &curr);

                size_t index = hash_64(curr.key)%(ths->bucket_count);

                if(new_data[index]){
                    list_push_front(new_data[index], {curr.key, curr.value});

                }else{
                    new_data[index] = New_List();

                    list_push_front(new_data[index], {curr.key, curr.value});
                }

            }
            Delete_List(ths->data[i]);

        }
    }
    free(ths->data);

    ths->data = new_data;
    update_load_factor(ths);


}




void insert(Hash_table* ths, const char* key, const char* value){
    size_t index = hash_64(key)%(ths->bucket_count);

    const char* res = NULL;
    
    if(get(ths, key, &res)){
        printf("Error you cant insert %s --%s. There is pair %s -- %s\n",key, value, key, res);
        return;
    }    


    if(ths->data[index]){
        list_push_front(ths->data[index], {key, value});

    }else{
        ths->data[index] = New_List();

        list_push_front(ths->data[index], {key, value});
    }
    
    ths->size++;
    update_load_factor(ths);
    
    if(ths->load_factor > max_load_factor){

        resize(ths);
    }
}



bool get(Hash_table* ths, const char* key, const char** result){

    size_t index = hash_64(key)%(ths->bucket_count);
    
    List* ptr = ths->data[index]; 

    if(!ptr){
        return false;
    }

    size_t count = ptr->size;

    // size_t count = 0;
    /* asm(".intel_syntax noprefix\n"
         "mov rbx, %2\n"            
         "shl rbx, 3\n"            
         "mov rax, [%1]\n"
         "add rax, rbx\n"            
         "mov rax, [rax]\n"            
        
        //  "add rbx, rax\n"            
           "add rax, 8\n"            
         "mov %0, [rax]\n"            

         :"=r"(count)                     
         :"r"(ths), "r"(index)              
         : "rax", "rbx"                      
    );
// */
    // printf("%lld %lld\n", res, ths->data[index]->size);

    Pair curr = {};

    for(size_t i = 1; i <= count; i++){
        list_get_value_by_index(ptr, i, &curr);


        if(strcmp(key, curr.key) == 0){
            *result = curr.value;
    
            return true;
        }
    }

    return false;
}


