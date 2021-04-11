#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include "hash_table.hpp"
#include "list.hpp"
#include "dictionary.hpp"
#include <immintrin.h>
#include <time.h>
// extern "C" list_codes list_get_value_by_index(List* ls, size_t ind, list_elem* res);

/*
list_codes list_get_value_by_index(List* ls, size_t ind, list_elem* res){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )

    long long node_size = sizeof(Node);


    printf("%lld", node_size);

    __asm__( ".intel_syntax noprefix\n "
             "mov rax, %3\n"
             "mov rbx, %1\n"
             "mul rbx\n"
             "mov rbx, [%0]\n"
            //  "\n"
             "add rbx, rax\n"

             "mov rax, [rbx]\n"
             "mov rdx, [rbx + 8]\n"

             "mov rbx, %2\n"

             "mov [rbx], rax\n"
             "mov [rbx + 8], rdx\n"
             
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
            :
            :"r"(ls), "r"(ind), "r"(res), "r"(node_size)
            :"rax", "rbx", "rdx"      );

    return LIST_OK;
}


*/











int main(){
    Dictionary dict = {};

    // char* input = (char*)calloc(80, sizeof(char));

    // fgets(input, 72, stdin);

    // input[strlen(input) - 1] = 0;


    load_dictionary(&dict, "dict.txt");

   /* char* line = (char*)calloc(32, sizeof(char));
    const char* test_line = "Hello peopleijefife";

    memcpy(line, test_line, strlen(test_line));
  
    clock_t begin = clock();

    // long long res = hash_64(line);
    // printf("%lld\n", res);
    for(int i = 0; i < 100000; i++){
        long long res = hash_64(line);
    }

    clock_t end = clock();

    printf("1: %d\n", end - begin);

    begin = clock();
    for(int i = 0; i < 100000; i++){
        long long res = hash(line);
    }

    end = clock();

    printf("2: %d\n", end - begin);



    begin = clock();
    for(int i = 0; i < 100000; i++){
        long long res = hash_8(line);
    }

    end = clock();

    printf("3: %d\n", end - begin);

*/
    build_hash_table(&dict);

    for(int i = 0; i < 100; i++){
        get_test(&dict);
    }
    // const char* result = NULL;

    // get_def(&dict, input, &result);
    // print_dictionary(&dict);
    // printf("%s", result);
    // fflush(stdout);
    unload_dictionary(&dict);


    // free(input);
}