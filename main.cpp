#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include "hash_table.hpp"
#include "list.hpp"
#include "dictionary.hpp"


int main(){
    Dictionary dict = {};

    load_dictionary(&dict, "dict.txt");

    build_hash_table(&dict);

    for(int i = 0; i < 100; i++){
        get_test(&dict);
    }
    // const char* result = NULL;

    // get_def(&dict, "японцы", &result);
    // print_dictionary(&dict);
    // printf("%s", result);
    unload_dictionary(&dict);



}