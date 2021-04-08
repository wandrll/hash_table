#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include "hash_table.hpp"
#include "list.hpp"
#include "dictionary.hpp"

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

    load_dictionary(&dict, "dict.txt");

    build_hash_table(&dict);

    for(int i = 0; i < 100; i++){
        get_test(&dict);
    }
    // const char* result = NULL;

    // get_def(&dict, "яхта", &result);
    // print_dictionary(&dict);
    // printf("%s", result);
    unload_dictionary(&dict);



}