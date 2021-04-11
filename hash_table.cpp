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

static unsigned long long hash(const char* line){
    
    unsigned long long hash = 5381;
  
    while (*line){
        hash = ((hash << 5) + hash) + *line;
        line++;
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

                size_t index = hash(curr.key)%(ths->bucket_count);

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
    // free(ths->values);
    // ths->keys = new_keys;
    ths->data = new_data;
    update_load_factor(ths);


}




void insert(Hash_table* ths, const char* key, const char* value){
    size_t index = hash(key)%(ths->bucket_count);
    
    if(ths->data[index]){
        list_push_front(ths->data[index], {key, value});

    }else{
        ths->data[index] = New_List();

        list_push_front(ths->data[index], {key, value});
    }
    
    ths->size++;
    update_load_factor(ths);
    
    if(ths->load_factor > max_load_factor){
        // printf("lfactor = %lg, size = %lld, buckets = %lld\n", ths->load_factor, ths->size, ths->bucket_count);

        resize(ths);
    }
}

// static inline size_t ret_count(Hash_table* ths, size_t index){
        // return ths->data[index]->size;
// }

bool get(Hash_table* ths, const char* key, const char** result){

    // unsigned first = clock();

    size_t index = hash(key)%(ths->bucket_count);
    
    if(!ths->data[index]){
        return false;
    }

    // size_t count = 0;
/*
    asm(".intel_syntax noprefix\n\t" 
        "mov rbx, %2\n"
        "shl rbx, 3\n"
        "mov rax, [%1]\n"
        "mov rax, [rax + rbx]\n"
        "mov %0, [rax + 8]\n"
        "\n"
        "\n"
        "\n"         
        :"=r"(count)                     
        :"r"(ths), "r"(index)              
        : "rax", "rbx"                     
    );
*/
    size_t count = ths->data[index]->size;

    Pair curr = {};

    for(size_t i = 1; i <= count; i++){
        list_get_value_by_index(ths->data[index], i, &curr);


        if(strcmp(key, curr.key) == 0){
            *result = curr.value;
    
            return true;
        }
    }

    return false;
}

