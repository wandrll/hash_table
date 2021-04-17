#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.hpp"
#include "hash_table.hpp"
#include <immintrin.h>

Hash_codes hash_table_constructor(Hash_table* ths){
    ths->data = (List**)calloc(1024, sizeof(List*));
    
    if(ths->data == NULL){
        return  HASH_CONSTRUCTOR_ERROR;
    }

    ths->load_factor = 0;
    ths->size = 0;
    ths->bucket_count = 1024;

    return HASH_OK;
}

static Hash_codes free_buckets(Hash_table* ths){
    if(ths->data == NULL){
        return HASH_DESTRUCTOR_ERROR;
    }

    for(int i = 0; i < ths->bucket_count; i++){
        if(ths->data[i]){
            Delete_List(ths->data[i]);
        }
    }

    return HASH_OK;
}

Hash_codes hash_table_destructor(Hash_table* ths){
    
    if(free_buckets(ths) != HASH_OK){
        return HASH_DESTRUCTOR_ERROR;
    }

    free(ths->data);
    return HASH_OK;
}


static inline void update_load_factor(Hash_table* ths){
    ths->load_factor = ((double)(ths->size))/((double)(ths->bucket_count));
}



extern "C" unsigned long long hash_64(const char* line){
    
    unsigned long long hash = 0;

    while (*((unsigned long long*)line)){
        hash = _mm_crc32_u64(hash, *((unsigned long long*)line));
        line+=8;
    }

    return hash;

}


extern "C"  int str_cmp(const char* a, const char* b){
    // return 0;
    return strcmp(a, b);
}




static Hash_codes resize(Hash_table* ths){
    List** new_data   = (List**)calloc(ths->bucket_count*2, sizeof(List*));
    
    if(new_data == NULL){
        return HASH_RESIZE_ERROR;
    }

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
                    list_push_back(new_data[index], {curr.key, curr.value});
                }else{
                    new_data[index] = New_List();
                    
                    if(new_data[index] == NULL){
                        return HASH_RESIZE_ERROR;
                    }

                    list_push_back(new_data[index], {curr.key, curr.value});
                }

            }
            Delete_List(ths->data[i]);

        }
    }
    free(ths->data);

    ths->data = new_data;
    update_load_factor(ths);

    return HASH_OK;
}




Hash_codes hash_table_insert(Hash_table* ths, const char* key, const char* value){

    size_t index = hash_64(key)%(ths->bucket_count);

    const char* res = NULL;
    
    res = hash_table_get(ths, key);

    if(res){
        printf("Error you cant insert %s --%s. There is pair %s -- %s\n",key, value, key, res);
        return HASH_INSERT_ERROR;
    }    


    if(ths->data[index]){
        list_push_back(ths->data[index], {key, value});

    }else{
        ths->data[index] = New_List();
        
        if(ths->data[index] == NULL){
            return HASH_INSERT_ERROR;
        }

        list_push_back(ths->data[index], {key, value});
    }
    
    ths->size++;
    update_load_factor(ths);

    if(ths->load_factor > max_load_factor){

        if(resize(ths) != HASH_OK){
            return HASH_RESIZE_ERROR;
        }
    }


    return HASH_OK;
}



/*
bool get(Hash_table* ths, const char* key, const char** result){

    size_t index = hash_64(key)%(ths->bucket_count);
    


    List* ptr = ths->data[index]; 

    if(!ptr){
        return false;
    }

    size_t count = ptr->size;


    Pair curr = {};
    
    // list_get_value_by_index(ptr, 1, &curr);
    // printf("%p %p\n", check(ths, key, result), curr.key);


    for(size_t i = 1; i <= count; i++){
        list_get_value_by_index(ptr, i, &curr);

        if(strcmp(key, curr.key) == 0){
            *result = curr.value;
    
            return true;
        }
    }

    return false;
}

*/
