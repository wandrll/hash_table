///\file
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "list.hpp"


List* New_List(){
    List* res = (List*)calloc(1, sizeof(List));
    list_constructor(res);
    return res;
}

void Delete_List(List* res){
    list_destructor(res);
    free(res);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////CONSTRUCTOR && DESTRUCTOR////////////////////////////////////////////////////

list_codes list_constructor(List* ls){
    assert(ls != NULL);

    const int first_count = 10;
    
    ls->data = (Node*)calloc(first_count, sizeof(Node));
    if(ls->data == NULL){
        return LIST_CONSTRUCTOR_ERROR;
    }else{
        ls->size = 0;
        ls->capacity = first_count - 1;    
        ls->free = 1;
        ls->is_ordered = true;

        for(int i = 1; i < first_count; i++){
            ls->data[i].prev = -1;
            ls->data[i].next = i+1;
        }
        
        return LIST_OK;
    }
}


list_codes list_destructor(List* ls){
    assert(ls != NULL);

    free(ls->data);
    ls->head = 0;
    ls->tail = 0;
    ls->size = 0;
    ls->capacity = 0;
    ls->free = 0;
    ls->is_ordered = false;
    
    return LIST_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////PUSH  BACK && FRONT//////////////////////////////////////////////////////////


list_codes list_push_back(List* ls, list_elem value){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    list_codes code = list_insert_by_index(ls, value, ls->tail);
    if(code != LIST_OK){
        return code;
    }
    return LIST_OK;
}

list_codes list_push_front(List* ls, list_elem value){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    list_codes code = list_insert_by_index(ls, value, 0); 
    if(code != LIST_OK){
        return code;
    }
    return LIST_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////POP BACK && FRONT////////////////////////////////////////////////////////////



list_codes list_pop_back(List* ls, list_elem* value){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    list_codes code = list_erase_by_index(ls, value, ls->tail);
    if(code != LIST_OK){
        return code;
    }
    return LIST_OK;
}

list_codes list_pop_front(List* ls, list_elem* value){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    list_codes code = list_erase_by_index(ls, value, ls->head);
    if(code != LIST_OK){
        return code;
    }
    return LIST_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////BACK && FRONT////////////////////////////////////////////////////////////////

list_codes list_back(List* ls, list_elem* value){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )

    if(ls->size == 0){
        return LIST_EMPTY;
    } 
    *value = ls->data[ls->tail].value;
    return LIST_OK;
}

list_codes list_front(List* ls, list_elem* value){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )

    if(ls->size == 0){
        return LIST_EMPTY;
    } 
    *value = ls->data[ls->head].value;
    return LIST_OK;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// INDEX FUNCTIONS//////////////////////////////////////////////////////////

list_codes list_linear_find_index_by_position_if_isnt_ordered(List* ls, size_t pos, size_t* res){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    if(pos > ls->size){
            return LIST_WRONG_INDEX;
    }
    if(ls->is_ordered){
        *res = pos;
    }else{
        size_t curr = 0;
        if(pos < ls->size/2){
            curr = 0;
            for(int i = 1; i <= pos; i++){
                curr = ls->data[curr].next;
            }
        }else{
            curr = 0;
            for(int i = 0; i <= ls->size - pos; i++){
                curr = ls->data[curr].prev;
            }
        }
        *res = curr;
    }
    return LIST_OK;
}

list_codes list_get_next_index(List* ls, size_t ind, size_t* res){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    
    if(ind == ls->tail){
        return LIST_OVERFLOW;
    }
    if(ls->data[ind].prev == -1){
        return LIST_WRONG_INDEX;
    }

    *res = ls->data[ind].next;
    return LIST_OK;
}

list_codes list_get_prev_index(List* ls, size_t ind, size_t* res){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    
    if(ind == ls->head){
        return LIST_UNDERFLOW;
    }
    if(ls->data[ind].prev == -1){
        return LIST_WRONG_INDEX;
    }

    *res = ls->data[ind].prev;
    return LIST_OK;
}




list_codes list_get_value_by_position(List* ls, size_t pos, list_elem* res){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    size_t ind = 0;
    list_codes code = list_linear_find_index_by_position_if_isnt_ordered(ls, pos, &ind);
    if(code != LIST_OK){
        return code;
    }else{
        return list_get_value_by_index(ls, ind, res);
    }
}


list_codes list_replace_value_by_index(List* ls, size_t ind, list_elem val){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )

    if(ind > ls->capacity){
        return LIST_OVERFLOW;
    }

    if(ls->data[ind].prev == -1){
        return LIST_WRONG_INDEX;
    }

    ls->data[ind].value = val;
    return LIST_OK;
}

list_codes list_replace_value_by_position(List* ls, size_t pos, list_elem val){
     IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    size_t ind = 0;
    list_codes code = list_linear_find_index_by_position_if_isnt_ordered(ls, pos, &ind);
    if(code != LIST_OK){
        return code;
    }else{
        return list_replace_value_by_index(ls, ind, val);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////INSERT///////////////////////////////////////////////////////////////////////

static list_codes list_increase_capacity(List* ls){
    Node* new_data = (Node*)calloc(2*(ls->capacity + 1), sizeof(Node));
    
    if(new_data == NULL){
        return LIST_INCREASE_ERROR;
    }
    
    for(int i = 0; i <= ls->capacity; i++){
        new_data[i].value = ls->data[i].value;
        new_data[i].prev = ls->data[i].prev;
        new_data[i].next = ls->data[i].next;
    }

    for(int i = ls->capacity+1; i < 2*(ls->capacity + 1); i++){
        new_data[i].prev = -1;
        new_data[i].next = i+1;
    }

    ls->capacity = 2*(ls->capacity + 1) - 1;
    free(ls->data);
    ls->data = new_data;
    
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    return LIST_OK;
}


list_codes list_insert_by_index(List* ls, list_elem value, size_t index){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    if(index > ls->capacity || ls->data[index].prev == -1 ){
        return LIST_WRONG_INDEX;
    }

    if(ls->tail != index){
        ls->is_ordered = false;
    }

    size_t next_free = ls->data[ls->free].next;
    ls->data[ls->free].value = value;
    
    ls->data[ls->free].prev = index;
    ls->data[ls->free].next = ls->data[index].next;

    ls->data[ls->data[ls->free].next].prev = ls->free;
    ls->data[ls->data[ls->free].prev].next = ls->free;
    ls->free = next_free;
    ls->size++;

    ls->head = ls->data[0].next;
    ls->tail = ls->data[0].prev;

    if(ls->size == ls->capacity){
        if(list_increase_capacity(ls) == LIST_INCREASE_ERROR){
            return LIST_INCREASE_ERROR;
        }
    }
    
    return LIST_OK;
}

list_codes list_insert_by_position(List* ls, list_elem value, size_t position){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )

    size_t ind = 0;
    list_codes code = list_linear_find_index_by_position_if_isnt_ordered(ls, position, &ind);
    if(code != LIST_OK){
        return code;
    }else{
        return list_insert_by_index(ls, value, ind);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////ERASE////////////////////////////////////////////////////////////////////////

static list_codes list_decrease_capacity(List* ls){
    Node* new_data = (Node*)calloc((ls->capacity + 1)/2, sizeof(Node));
    
    assert(ls->is_ordered != false);

    if(new_data == NULL){
        return LIST_INCREASE_ERROR;
    }
    
    size_t curr = 0;
    
    for(int i = 0; i <= ls->size; i++){
        new_data[i].value = ls->data[i].value;
        new_data[i].prev = ls->data[i].prev;
        new_data[i].next = ls->data[i].next;
    }

    for(int i = ls->size + 1; i < (ls->capacity + 1)/2; i++){
        new_data[i].prev = -1;
        new_data[i].next = i+1;
    }
    
    ls->capacity = (ls->capacity + 1)/2 - 1;
    free(ls->data);
    ls->data = new_data;

    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    return LIST_OK;
}


list_codes list_erase_by_index(List* ls, list_elem* value, size_t index){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )
    
    if(index == 0 || index >= ls->capacity + 1 || ls->data[index].prev == -1){
        return LIST_WRONG_INDEX;
    }

    if(ls->tail != index){
        ls->is_ordered = false;
    }

    ls->data[ls->data[index].next].prev = ls->data[index].prev;
    ls->data[ls->data[index].prev].next = ls->data[index].next;

    *value = ls->data[index].value;
    ls->data[index].next = ls->free;
    ls->data[index].prev = -1;
    // ls->data[index].value = {0, 0};
    ls->free = index;
    
    ls->size--;
    ls->head = ls->data[0].next;
    ls->tail = ls->data[0].prev;
    
     if(ls->size < ls->capacity/4 && ls->is_ordered){
        if(list_decrease_capacity(ls) == LIST_DECREASE_ERROR){
            return LIST_DECREASE_ERROR;
        }
    }
    

    return LIST_OK;
}


list_codes list_erase_by_position(List* ls, list_elem* value, size_t position){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )

    size_t ind = 0;
    list_codes code = list_linear_find_index_by_position_if_isnt_ordered(ls, position, &ind);
    if(code != LIST_OK){
        return code;
    }else{
        return list_erase_by_index(ls, value, ind);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// MAKE ORDER ////////////////////////////////////////////////////////////////

list_codes list_create_order(List* ls){
    IF_DEBUG_ON(
        if(!list_validation(ls, create_log_data(__FILE__, __FUNCTION__, __LINE__))){
            return LIST_CORRUPTED;

        }
    )

    Node* new_data = (Node*)calloc((ls->capacity + 1), sizeof(Node));
    
    if(new_data == NULL){
        return LIST_ORDER_ERROR;
    }
    
    size_t curr = 0;
    
    new_data[curr].value = ls->data[0].value;
    new_data[curr].prev = ls->size;
    new_data[curr].next = (curr + 1)%(ls->size + 1);
    curr++;
    
    for(int i = ls->head; i != 0; i = ls->data[i].next){

        new_data[curr].value = ls->data[i].value;
        new_data[curr].prev = (curr - 1);
        new_data[curr].next = (curr + 1)%(ls->size + 1);
        curr++;
    }

    ls->is_ordered = true;
    for(int i = curr; i < (ls->capacity + 1); i++){
        new_data[i].prev = -1;
        new_data[i].next = i+1;
    }
    
    free(ls->data);

    ls->data = new_data;

    ls->head = ls->data[0].next;
    ls->tail = ls->data[0].prev;
    


    ls->free = ls->size + 1;
    return LIST_OK;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// DUMP //////////////////////////////////////////////////////////////////////

void list_dump_diag(List* ls, const char* file){
    assert(ls != NULL);
    assert(ls->data != NULL);
    assert(file != NULL);

    FILE* fp = fopen("tmp.gv", "w");
    fprintf(fp,"digraph G{\n");
    fprintf(fp, "   graph[splines =true];\n");
    fprintf(fp, "metadata[label = \"size:%ld\\ncapacity:%ld\\nhead:%ld\\ntail:%ld\\nfree:%ld  \"];",ls->size, ls->capacity, ls->head, ls->tail, ls->free);
    for(int i = 0; i <= ls->capacity; i++){
        fprintf(fp, "_%d[shape=record,label=\" val = %lg | {<prev> prev = %d | <next> next = %d} | ind = %d\" style = filled fillcolor=\"#F3FDC9\"];\n ",i, ls->data[i].value, ls->data[i].prev, ls->data[i].next, i);
        if(ls->data[i].prev == -1){
            fprintf(fp, "_%d[style = filled fillcolor=\"#8BF696\"];\n", i);
        }
    }


    fprintf(fp, "_0[style = filled fillcolor=\"#A4BEF2\"];\n");

    fprintf(fp, "_%d[shape=record,label=\" val = %lg | {<prev> prev = %d | <next> next = %d} | ind = %d | HEAD\" style = filled fillcolor=\"#AF68C9\"];\n ",ls->head, ls->data[ls->head].value, ls->data[ls->head].prev, ls->data[ls->head].next, ls->head);
        
    fprintf(fp, "_%d[shape=record,label=\" val = %lg | {<prev> prev = %d | <next> next = %d} | ind = %d | TAIL\" style = filled fillcolor=\"#F9E7BC\"];\n ",ls->tail, ls->data[ls->tail].value, ls->data[ls->tail].prev, ls->data[ls->tail].next, ls->tail);


    for(int curr = 0; curr <= ls->capacity; curr++){
        if(ls->data[curr].prev != -1){
            fprintf(fp, "_%d->_%d\n", curr, ls->data[curr].next);
            fprintf(fp, "_%d->_%d\n", curr, ls->data[curr].prev);

        }else{
                
            fprintf(fp, "_%d->_%d\n", curr, ls->data[curr].next);
                
        }
    }

    size_t curr = ls->head;

    for(int i = 0; i < ls->size; i++){
        fprintf(fp, "%d[shape=record label=\"pos = %d \"]",i, i+1);
        fprintf(fp, "%d->_%d[minlen=30]\n", i, curr);
        if(curr != -1 && curr <= ls->capacity){
            curr = ls->data[curr].next;
        }
    }

    fprintf(fp,"}\n");
    fclose(fp);
    char* str = (char*)calloc(23+strlen(file), sizeof(char));
    strcat(str, "circo -Tpdf tmp.gv -o");
    strcat(str, file);
    system(str);
    free(str);
    remove("tmp.gv");
}


void list_dump_file(List* ls, const char* file){
    FILE* fp = fopen(file, "a");
    fprintf(fp,"\ncapacity: %ld\n size: %ld\n head: %ld\n tail: %ld\n free: %ld\n is ordered: %d\n", ls->capacity,
                                                                                                   ls->size,
                                                                                                   ls->head, 
                                                                                                   ls->tail, 
                                                                                                   ls->free, 
                                                                                                   ls->is_ordered);


    for(int curr = 0; curr <= ls->capacity; curr++){
        fprintf(fp, "%5lg ", ls->data[curr].value);
    }
    fprintf(fp, "\n");

    for(int curr = 0; curr <= ls->capacity; curr++){
        fprintf(fp, "%5d ", ls->data[curr].next);
    }
    fprintf(fp, "\n");


    for(int curr = 0; curr <= ls->capacity; curr++){
        fprintf(fp, "%5d ", ls->data[curr].prev);
    }
    fprintf(fp,"\n-------------------------------------------\n");
    fclose(fp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// VALIDATION //////////////////////////////////////////////////////////////////////


static bool is_straight_cycle_ok(List* ls){
    size_t curr = ls->head;
    for(int i = 0; i < ls->size; i++){
        if(curr == -1 || curr == 0 || curr > ls->capacity){
            return false;
        }
        curr = ls->data[curr].next;
    }

    if(curr != 0|| ls->data[curr].prev != ls->tail || ls->data[curr].next != ls->head){
        return false;
    }

    return true;
}

static bool is_reversed_cycle_ok(List* ls){
    size_t curr = ls->tail;
    for(int i = 0; i < ls->size; i++){
        if(curr == -1 || curr == 0 || curr > ls->capacity){
            return false;
        }
        curr = ls->data[curr].prev;
    }

    if(curr != 0 || ls->data[curr].prev != ls->tail || ls->data[curr].next != ls->head){
        return false;
    }
    
    return true;

}

static bool is_pointer_valid(void* data){
    if((void*)data < (void*)4096){
        return false;
    }else{
        return true;
    }
}

static bool is_parametrs_ok(List* ls){
    if(ls->size > ls->capacity || ls-> head > ls->capacity || ls->tail > ls->capacity || ls->free > ls->capacity){
        return false;
    }else{
        return true;
    }
}

static void print_log_data(FILE* fp, Log_data* log){
    fprintf(fp, "file:%s\n function: %s\n line: %d\n", log->file, log->func, log->line);
}


bool list_validation(List* ls, Log_data* log){
    FILE* fp = fopen("error_log.txt", "a");
    if(!is_pointer_valid((void*)ls)){
        print_log_data(fp, log);
        fprintf(fp, "\nNon valid list pointer: %p\n",ls);
        free(log);
        fclose(fp);
        return false;
    }
    if(!is_pointer_valid((void*)ls->data)){
        print_log_data(fp, log);
        fprintf(fp, "\nNon valid list_data pointer: %p\n",ls->data);
        free(log);
        fclose(fp);
        return false;
    }
   
    if(!is_parametrs_ok(ls)){
        print_log_data(fp, log);
        fprintf(fp, "\nParametrs of list have corrupted: size: %ld\ncapacity:%ld\nhead:%ld\ntail:%ld\nfree:%ld\n",ls->size, ls->capacity, ls->head, ls->tail, ls->free);
        free(log);
        fclose(fp);
        return false;
    }


    if(!is_reversed_cycle_ok(ls) || !is_straight_cycle_ok(ls)){
        print_log_data(fp, log);
        fprintf(fp, "\nCycle in list was corrupted, check error.pdf");
        list_dump_diag(ls, "error.pdf");
        free(log);
        fclose(fp);
        list_dump_file(ls, "error_log.txt");
        return false;
    }

    free(log);
    fclose(fp);
    return true;

}


Log_data* create_log_data(const char* file, const char* func, int line){
    Log_data* lg = (Log_data*)calloc(1, sizeof(Log_data));
    lg->file = file;
    lg->func = func;
    lg->line = line;
    return lg;
}


