#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list.hpp"

const double max_load_factor = 0.75;


struct Hash_table{
    List** data;

    long long size;
    long long bucket_count;

    size_t* sizes;

    double load_factor;

};

void hash_table_constructor(Hash_table* ths);

void hash_table_destructor(Hash_table* ths);

void hash_table_insert(Hash_table* ths, const char* key, const char* value);

extern "C" const char* hash_table_get(Hash_table* ths, const char* key);


