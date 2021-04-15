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

void constructor(Hash_table* ths);

void destructor(Hash_table* ths);

void insert(Hash_table* ths, const char* key, const char* value);

bool get(Hash_table* ths, const char* key, const char** result);

extern "C" const char* check(Hash_table* ths, const char* key);


