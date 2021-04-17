#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list.hpp"

const double max_load_factor = 0.75;

enum Hash_codes{
    HASH_OK,
    HASH_CONSTRUCTOR_ERROR,
    HASH_DESTRUCTOR_ERROR,
    HASH_INSERT_ERROR,
    HASH_RESIZE_ERROR
};


struct Hash_table{
    List** data;

    long long size;
    long long bucket_count;


    double load_factor;

};

Hash_codes hash_table_constructor(Hash_table* ths);

Hash_codes hash_table_destructor(Hash_table* ths);

Hash_codes hash_table_insert(Hash_table* ths, const char* key, const char* value);

extern "C" const char* hash_table_get(Hash_table* ths, const char* key);


