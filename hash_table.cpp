#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.hpp"
#include "hash_table.hpp"


void constructor(Hash_table* ths){
    ths->values = (List**)calloc(1024, sizeof(List*));
    ths->keys = (List**)calloc(1024, sizeof(List*));

    ths->load_factor = 0;
    ths->size = 0;
    ths->bucket_count = 1024;

}

static void free_buckets(Hash_table* ths){

    for(int i = 0; i < ths->bucket_count; i++){
        if(ths->keys[i]){
            Delete_List(ths->keys[i]);
            Delete_List(ths->values[i]);
        }
    }
}

void destructor(Hash_table* ths){
    free_buckets(ths);
    free(ths->keys);
    free(ths->values);
}


static inline void update_load_factor(Hash_table* ths){
    ths->load_factor = ((double)(ths->size))/((double)(ths->bucket_count));
}

static unsigned long long hash(const char* line){
    
    unsigned long long hash = 5381;
    int c = 0;

    while (c = *line){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        line++;
    }

    return hash;
}


static void resize(Hash_table* ths){
    List** new_keys   = (List**)calloc(ths->bucket_count*2, sizeof(List*));
    List** new_values = (List**)calloc(ths->bucket_count*2, sizeof(List*));

    size_t count = ths->bucket_count;
    ths->bucket_count = ths->bucket_count*2;

    for(size_t i = 0; i < count; i++){
        
        if(ths->keys[i]){
            const size_t curr_count = ths->keys[i]->size;
            for(int j = 0; j < curr_count; j++){

                const char* curr_key = NULL;
                list_pop_back(ths->keys[i], &curr_key);

                const char* curr_val = NULL;
                list_pop_back(ths->values[i], &curr_val);

                size_t index = hash(curr_key)%(ths->bucket_count);

                if(new_keys[index]){
                    list_push_front(new_keys[index], curr_key);
                    list_push_front(new_values[index], curr_val);

                }else{
                    new_keys[index] = New_List();
                    new_values[index] = New_List();

                    list_push_front(new_keys[index], curr_key);
                    list_push_front(new_values[index], curr_val);
                }

            }
            Delete_List(ths->keys[i]);
            Delete_List(ths->values[i]);

        }
    }
    free(ths->keys);
    free(ths->values);
    ths->keys = new_keys;
    ths->values = new_values;
    update_load_factor(ths);


}




void insert(Hash_table* ths, const char* key, const char* value){
    size_t index = hash(key)%(ths->bucket_count);
    
    if(ths->keys[index]){
        list_push_front(ths->keys[index], key);
        list_push_front(ths->values[index], value);

    }else{
        ths->keys[index] = New_List();
        ths->values[index] = New_List();

        list_push_front(ths->keys[index], key);
        list_push_front(ths->values[index], value);
    }
    
    ths->size++;
    update_load_factor(ths);
    
    if(ths->load_factor > max_load_factor){
        resize(ths);
    }
}


bool get(Hash_table* ths, const char* key, const char** result){
    size_t index = hash(key)%(ths->bucket_count);
    
    if(!ths->keys[index]){
        return false;
    }

    size_t count = ths->keys[index]->size;

    for(size_t i = 1; i <= count; i++){
        const char* curr = NULL;
        list_get_value_by_index(ths->keys[index], i, &curr);

        if(strcmp(key, curr) == 0){
            list_get_value_by_index(ths->values[index], i, result);
            
            return true;
        }
    }
    
    return false;
}

