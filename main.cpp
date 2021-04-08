#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include "hash_table.hpp"

class Dictionary{
    public:
    void load_dictionary(const char* file){

        size_t size = file_size(file);
        

        this->buffer = (char*)calloc(size+1, sizeof(char));        
        assert(this->buffer != NULL);

        FILE* fp = fopen (file, "rb");
        assert(fp != NULL);
        
        fread(this->buffer, sizeof(char), size, fp);
        fclose(fp);




        this->size = count_of_symbol(this->buffer, '\n');

        this->word = (char**)calloc(this->size, sizeof(char*));
        this->def  = (char**)calloc(this->size, sizeof(char*));

        parse_buffer();
    
    }


    void build_hash_table(){
        this->table = (Hash_table<char*>*)calloc(1, sizeof(Hash_table<char*>));
        this->table->constructor();

        for(int i = 0; i < this->size; i++){
            this->table->insert(this->word[i], this->def[i]);
        }

    }

    bool get_def(const char* word, char** res){
        return this->table->get(word, res);
    }

    void get_test(){
        char* result = NULL;
        for(int i = 0; i < this->size; i++){
            bool res = get_def(this->word[i], &result);
        }
    }

    void unload_dictionary(){
        this->table->destructor();
        free(this->table);
        free(this->buffer);
        free(this->word);
        free(this->def);
        this->size = 0;
        this->buffer = NULL;
        this->word = NULL;
        this->def = NULL;
        this->table = NULL;
    }

    void print_dictionary(){
        for(int i = 0; i < this->size; i++){
            printf("%s - %s \n", this->word[i], this->def[i]);
        }
    }

    
    
    private:
    char** word;
    char** def;
    size_t size;

    Hash_table<char*>* table;
    
    void parse_buffer(){
        char* currleft  = this->buffer;
        char* currright = this->buffer;

        size_t curr_word = 0;

        while(*currright != 0){
            switch(*currright){
                case '=':{
                    *currright = 0;
                    this->word[curr_word] = currleft;
                    
                    currright++;
                    currleft = currright;
                    
                    break;
                }

                case '\n':{
                    *currright = 0;
                    this->def[curr_word] = currleft;
                    
                    curr_word++;
                    
                    currright++;
                    currleft = currright;

                    break;
                }

                case '\r':{
                    *currright = 0;
                    currright++;
                    break;
                }

                default:{
                    currright++;
                    break;
                }
            }
        }
    }

    
    
    size_t count_of_symbol(const char* buf, char c){
        size_t count = 0;
        while(*buf != 0){
            buf = strchr(buf, c);
            count++;
            buf++;
        }
        return count;
    }

    size_t file_size(const char* file){
        assert(file != NULL);

        struct stat st = {};
        stat(file, &st);
        return st.st_size;
    }

    char* buffer;
};





int main(){
    Dictionary dict = {};

    dict.load_dictionary("dict.txt");

    dict.build_hash_table();

    for(int i = 0; i < 10; i++){
        dict.get_test();
    }
    // dict.print_dictionary();
    // printf("%s", result);
    dict.unload_dictionary();



}