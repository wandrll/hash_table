#pragma once
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include "list.hpp"

const double max_load_factor = 0.75;


template<class Type>
class Hash_table{
public:
    class Pair{
    public:

    const char* key;
    Type value;
 
    Pair& operator=(int a){
        this->key = 0;
        this->value = 0;
        return *this;
    }

    };

void constructor(){
    this->data = (List<Pair>**)calloc(1024, sizeof(List<Pair>*));

    this->occupied_buckets = New<size_t>();
    this->load_factor = 0;
    this->size = 0;
    this->bucket_count = 1024;

}

void destructor(){
    free_buckets();
    this->occupied_buckets->destructor();
    free(this->data);
    free(this->occupied_buckets);
}

void insert(const char* key, const Type& value){
    size_t index = hash(key)%(this->bucket_count);
    if(this->data[index]){
        this->data[index]->push_front({key, value});
    }else{
        this->data[index] = New<Pair>();
        this->data[index]->push_front({key, value});
        this->occupied_buckets->push_back(index);
    }
    
    this->size++;
    update_load_factor();
    
    if(this->load_factor > max_load_factor){
        resize();
    }
}

void remove(const char* key){
    
}

bool get(const char* key, Type* result){
    size_t index = hash(key)%(this->bucket_count);
    size_t count = this->data[index]->count();

    for(int i = 1; i <= count; i++){
        Pair curr = this->data[index]->get_value_by_index(i);
        if(strcmp(key, curr.key) == 0){
            *result = curr.value;
            return true;
        }
    }
    return false;
}


private:

List<Pair>** data;
List<size_t>* occupied_buckets;
long long size;

long long bucket_count;

double load_factor;

void resize(){
    List<Pair>** new_res = (List<Pair>**)calloc(this->bucket_count*2, sizeof(List<Pair>*));

    this->bucket_count = this->bucket_count*2;
    size_t count_of_oc_buck = this->occupied_buckets->count();

    for(int i = 0; i < count_of_oc_buck; i++){
        size_t curr_ind = this->occupied_buckets->pop_back();
        size_t count = this->data[curr_ind]->count();
        for(int j = 0; j < count; j++){
            
            Pair curr = this->data[curr_ind]->pop_back();
            size_t index = hash(curr.key)%(this->bucket_count);

            if(new_res[index]){
                new_res[index]->push_front(curr);
            }else{
                new_res[index] = New<Pair>();
                new_res[index]->push_front(curr);
                this->occupied_buckets->push_front(index);
            }

        }
        Delete(this->data[curr_ind]);
    }
    free(this->data);
    this->data = new_res;
    update_load_factor();


}


void update_load_factor(){
    this->load_factor = ((double)(this->size))/((double)(this->bucket_count));
}

unsigned long long hash(const char* line){
    
    unsigned long long hash = 5381;
    int c = 0;

    while (c = *line){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        line++;
    }

    return hash;
}

void free_buckets(){
    int n = this->occupied_buckets->count();
    for(int i = 0; i < n; i++){
        size_t curr_ind = this->occupied_buckets->pop_back();
        Delete(this->data[curr_ind]);
    }
}

};