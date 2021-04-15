#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include "hash_table.hpp"
#include "list.hpp"
#include "dictionary.hpp"

static size_t count_of_symbol(const char* buf, char c){
    size_t count = 0;
    while(*buf != 0){
        buf = strchr(buf, c);
        count++;
        buf++;
    }
    return count;
}

static size_t file_size(const char* file){
    assert(file != NULL);

    struct stat st = {};
    stat(file, &st);
    return st.st_size;
}


void allign_input_buffer(Dictionary* ths, size_t size){
    char* new_buffer = (char*)calloc(size, sizeof(char));

    char* curr = new_buffer;

    for(size_t i = 0; i < ths->size; i++){
        int length_word = strlen(ths->word[i]);
        int length_def  = strlen(ths-> def[i]);

        strcpy(curr, ths->word[i]);
        ths->word[i] = curr;

        curr += length_word;
        curr += (8 + (8 - (length_word & 7)) * ((length_word & 7) != 0));
        
     

        strcpy(curr, ths->def[i]);
        ths->def[i] = curr;

        curr += length_def;
        curr += (8 + (8 - (length_def & 7)) * ((length_def & 7) != 0));

    }

    free(ths->buffer);

    ths->buffer = new_buffer;

}




static void parse_buffer(Dictionary* ths){
    char* currleft  = ths->buffer;
    char* currright = ths->buffer;

    size_t curr_count = 0;

    size_t result = 0;

    size_t curr_word = 0;

    while(*currright != 0){
        switch(*currright){
            case '=':{
                *currright = 0;
                ths->word[curr_word] = currleft;

                currright++;
                currleft = currright;
                

                result += (1 + (curr_count >> 3) + ((curr_count & 7) != 0))*8;
                curr_count = 0;

                break;
            }
            case '\n':{
                *currright = 0;
                ths->def[curr_word] = currleft;

                curr_word++;
                currright++;
                currleft = currright;


                result += (1 + (curr_count >> 3) + ((curr_count & 7) != 0))*8;
                curr_count = 0;

                break;
            }
            
            default:{
                currright++;
                curr_count++;
                break;
            }
        }
    }


    allign_input_buffer(ths, result);

}


void load_dictionary(Dictionary* ths, const char* file){

    size_t size = file_size(file);

    ths->buffer = (char*)calloc(size+1, sizeof(char));        
    assert(ths->buffer != NULL);
    FILE* fp = fopen (file, "rb");
    assert(fp != NULL);
    
    fread(ths->buffer, sizeof(char), size, fp);
    fclose(fp);

    ths->size = count_of_symbol(ths->buffer, '\n');

    ths->word = (char**)calloc(ths->size, sizeof(char*));
    ths->def  = (char**)calloc(ths->size, sizeof(char*));
    
    parse_buffer(ths);

}


void unload_dictionary(Dictionary* ths){
    
    if(ths->table){
        hash_table_destructor(ths->table);
    }

    free(ths->table);
    free(ths->buffer);
    free(ths->word);
    free(ths->def);

    ths->size = 0;
    ths->buffer = NULL;
    ths->word = NULL;
    ths->def = NULL;
    ths->table = NULL;
}



void print_dictionary(Dictionary* ths){
    for(int i = 0; i < ths->size; i++){
        printf("%s - %s \n", ths->word[i], ths->def[i]);
    }
}




void build_hash_table(Dictionary* ths){
    ths->table = (Hash_table*)calloc(1, sizeof(Hash_table));
    hash_table_constructor(ths->table);

    for(int i = 0; i < ths->size; i++){
        hash_table_insert(ths->table, ths->word[i], ths->def[i]);
    }

}

bool get_def(Dictionary* ths, const char* word, const char** res){
    return hash_table_get(ths->table, word);
}

void print_lengths(Hash_table* ths){
    for(int i = 0; i < ths->bucket_count; i++){
        if(!ths->data[i]){
            printf("0\n");
        }else{
            printf("%lld\n",ths->data[i]->size);
        }
    }
}


void get_test(Dictionary* ths){
    const char* result = NULL;
    for(int i = 0; i < ths->size; i++){
        
        result = hash_table_get(ths->table, ths->word[i]);

        if(!result || strcmp(result, ths->def[i]) != 0){
            printf("Error: excpected:\n %s -- %s, but \n %s -- %s", ths->word[i], ths->def[i], ths->word[i], result);
        }
    }

}