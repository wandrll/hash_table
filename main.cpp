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

    // print_dictionary(&dict);

    get_correct_test(&dict);
    
    get_test(&dict);
    

    unload_dictionary(&dict);

}