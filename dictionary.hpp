#pragma once

#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include "hash_table.hpp"
#include "list.hpp"


/*
extern "C" char* List<Hash_table::Pair>::get_value_val_by_index(size_t ind){
    
}
*/
struct Dictionary{
    char** word;
    char** def;
    size_t size;

    Hash_table* table;
    
    char* buffer;
};

void load_dictionary(Dictionary* ths, const char* file);

void unload_dictionary(Dictionary* ths);

void print_dictionary(Dictionary* ths);

void build_hash_table(Dictionary* ths);

bool get_def(Dictionary* ths, const char* word, const char** res);

void get_test(Dictionary* ths);