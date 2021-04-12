#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include "hash_table.hpp"
#include "list.hpp"
#include "dictionary.hpp"
#include <time.h>




int main(){
    Dictionary dict = {};
    
    load_dictionary(&dict, "dict.txt");


    build_hash_table(&dict);

    for(int i = 0; i < 1000; i++){
        get_test(&dict);
    }

    unload_dictionary(&dict);

}