/**
 * \mainpage
 * \author Wandrl
 * \version 0.9
 * \date 2020.11.09\n
 * Double linked list.\n
 * There are pop, push, insert, erase functions by phisical indexes.\n
 */
#pragma once
#include <cstdio>
#include <cassert>
#include <cstdlib>
///\file






/**
 * Doubly linked list
 * \param data array of data
 * \param size size of list
 * \param capacity capacity of list
 * \param free index of free cell
 * \param head index of list head
 * \param tail index of tail list
 * \param is_ordered is list ordered by positions
 */
template<class Type>
struct List{

    struct Node{
        Type value;
        int next;
        int prev;
    };


    /**
     * Constructor
     * \param ls pointer on list
     */

    int count(){
        return this->size;
    }


    void constructor(const int first_count = 10){
    
        this->data = (Node*)calloc(first_count, sizeof(Node));
        if(this->data == NULL){
            assert(0);
        }else{
            this->size = 0;
            this->capacity = first_count - 1;    
            this->free_ = 1;
            this->is_ordered = true;

            for(int i = 1; i < first_count; i++){
                this->data[i].prev = -1;
                this->data[i].next = i+1;
            }
        }
    }

    /**
     * Destructor
     * \param ls pointer on list
     */
    void destructor(){
        free(this->data);
        this->head = 0;
        this->tail = 0;
        this->size = 0;
        this->capacity = 0;
        this->free_ = 0;
        this->is_ordered = false;
    }

    /**
     * Add element in end of list
     * \param ls pointer on list
     * \param value element to push
     */
    void push_back(const Type& value){
        insert_after_by_index(value, this->tail);
    }

    /**
     * Add element in begin of list
     * \param ls pointer on list
     * \param value element to push
     */
    void push_front(const Type& value){
        insert_after_by_index(value, 0); 
    }

    /**
     * Remove and return element from end of list
     * \param ls pointer on list
     * \param value pointer where to save element
     */
    Type pop_back(){
        return erase_by_index(this->tail);
    }


    /**
     * Remove and return element from begin of list
     * \param ls pointer on list
     * \param value pointer where to save element
     */
    Type pop_front(){
        return erase_by_index(this->head);
    }

    /**
     * Get last element of list
     * \param ls pointer on list
     * \param value pointer where to save element
     */
    Type back(){
        return this->data[this->tail].value;
    }

    /**
     * Get first element of list
     * \param ls pointer on list
     * \param value pointer where to save element
     */
    Type front(){
        return this->data[this->head].value;
    }

    /**
     * Linear search of index in array by position in list
     * \param ls pointer on list
     * \param num position
     * \param res pointer where to save index
     */
    size_t linear_find_index_by_position_if_isnt_ordered(size_t pos){
        if(pos > this->size){
            assert(0);
        }
        if(this->is_ordered){
            return pos;
        }else{
            size_t curr = 0;
            if(pos < this->size/2){
                curr = 0;
                for(int i = 1; i <= pos; i++){
                    curr = this->data[curr].next;
                }
            }else{
                curr = 0;
                for(int i = 0; i <= this->size - pos; i++){
                    curr = this->data[curr].prev;
                }
            }
            return curr;
        }
    }

    /**
     * Get next index by index
     * \param ls pointer on list
     * \param ind index
     * \param res pointer where to save next index
     */
    size_t get_next_index(size_t ind){
        if(ind == this->tail){
            return 0;
        }
        if(this->data[ind].prev == -1){
            return 0;
        }

        return this->data[ind].next;
    }

    /**
     * Get previous index by index
     * \param ls pointer on list
     * \param ind index
     * \param res pointer where to save previous index
     */
    size_t get_prev_index(size_t ind){
        if(ind == this->head){
            assert(0);
        }
        if(this->data[ind].prev == -1){
            assert(0);
        }

        return this->data[ind].prev;
    }

    /**
     * Get element by index
     * \param ls pointer on list
     * \param ind index
     * \param res pointer where to save value
     */
    Type get_value_by_index(size_t ind){
        if(this->data[ind].prev == -1){
            assert(0);
        }

        return this->data[ind].value;
    }

    /**
     * Repalce element by index
     * \param ls pointer on list
     * \param ind index
     * \param val element that will replace
     */
    void replace_value_by_index(size_t ind, const Type& val){
        if(ind > this->capacity){
            assert(0);
        }

        if(this->data[ind].prev == -1){
            assert(0);
        }

        this->data[ind].value = val;
    }

    /**
     * Insert element after elemetn with index
     * \param ls pointer on list
     * \param value value that will insert
     * \param index index of element, after which the insert will be
     */
    void insert_after_by_index(const Type& value, size_t index){
        // printf("----------\n %p", this);
        // fflush(stdout);
        if(index > this->capacity || this->data[index].prev == -1 ){
            assert(0);
        }
        //  printf("----=+++-\n");
        // fflush(stdout);

        if(this->tail != index){
            this->is_ordered = false;
        }

        size_t next_free_ = this->data[this->free_].next;
        this->data[this->free_].value = value;

        this->data[this->free_].prev = index;
        this->data[this->free_].next = this->data[index].next;
        this->data[this->data[this->free_].next].prev = this->free_;
        this->data[this->data[this->free_].prev].next = this->free_;
        this->free_ = next_free_;
        this->size++;

        this->head = this->data[0].next;
        this->tail = this->data[0].prev;

        if(this->size == this->capacity){
            increase_capacity();
        }
    }

    /**
     * Insert element before  elemetn with index
     * \param ls pointer on list
     * \param value value that will insert
     * \param index index of element, after which the insert will be
     */
    void insert_before_by_index(const Type& value, size_t index){
        if(index > this->capacity || this->data[index].prev == -1 || index == 0){
            assert(0);
        }

        insert_after_by_index(value, this->data[index].prev);
    }


    /**
     * Erase and save element with index
     * \param ls pointer on list
     * \param value pointer where to save element
     * \param index index of element, whihc must be erased
     */
    Type erase_by_index(size_t index){
        if(index == 0 || index >= this->capacity + 1 || this->data[index].prev == -1){
            assert(0);
        }

        if(this->tail != index){
            this->is_ordered = false;
        }

        this->data[this->data[index].next].prev = this->data[index].prev;
        this->data[this->data[index].prev].next = this->data[index].next;

        Type res = this->data[index].value;
        this->data[index].next = this->free_;
        this->data[index].prev = -1;
        this->data[index].value = 0;
        this->free_ = index;

        this->size--;
        this->head = this->data[0].next;
        this->tail = this->data[0].prev;

        if(this->size < this->capacity/4 && this->is_ordered){
            decrease_capacity();
        }
        return res;
    }

    /**
     * Order list by positions
     * \param ls pointer on list
     */
    void create_order(){
        Node* new_data = (Node*)calloc((this->capacity + 1), sizeof(Node));
    
        if(new_data == NULL){
            assert(0);
        }

        size_t curr = 0;

        new_data[curr].value = this->data[0].value;
        new_data[curr].prev = this->size;
        new_data[curr].next = (curr + 1)%(this->size + 1);
        curr++;

        for(int i = this->head; i != 0; i = this->data[i].next){

            new_data[curr].value = this->data[i].value;
            new_data[curr].prev = (curr - 1);
            new_data[curr].next = (curr + 1)%(this->size + 1);
            curr++;
        }

        this->is_ordered = true;
        for(int i = curr; i < (this->capacity + 1); i++){
            new_data[i].prev = -1;
            new_data[i].next = i+1;
        }

        free(this->data);

        this->data = new_data;

        this->head = this->data[0].next;
        this->tail = this->data[0].prev;



        this->free_ = this->size + 1;
    }

    

    private:

    Node* data;
    size_t size;
    size_t capacity;
    size_t free_;
    size_t head;
    size_t tail;
    bool is_ordered;
    
    void increase_capacity(){
        Node* new_data = (Node*)calloc(2*(this->capacity + 1), sizeof(Node));

        if(new_data == NULL){
            assert(0);
        }

        for(int i = 0; i <= this->capacity; i++){
            new_data[i].value = this->data[i].value;
            new_data[i].prev = this->data[i].prev;
            new_data[i].next = this->data[i].next;
        }

        for(int i = this->capacity+1; i < 2*(this->capacity + 1); i++){
            new_data[i].prev = -1;
            new_data[i].next = i+1;
        }

        this->capacity = 2*(this->capacity + 1) - 1;
        free(this->data);
        this->data = new_data;

    }


    void decrease_capacity(){
        Node* new_data = (Node*)calloc((this->capacity + 1)/2, sizeof(Node));
        assert(this->is_ordered != false);
        if(new_data == NULL){
            assert(0);
        }

        size_t curr = 0;

        for(int i = 0; i <= this->size; i++){
            new_data[i].value = this->data[i].value;
            new_data[i].prev =  this->data[i].prev;
            new_data[i].next =  this->data[i].next;
        }

        for(int i = this->size + 1; i < (this->capacity + 1)/2; i++){
            new_data[i].prev = -1;
            new_data[i].next = i+1;
        }

        this->capacity = (this->capacity + 1)/2 - 1;
        free(this->data);
        this->data = new_data;

    }

   
};


template<class Type>
List<Type>* New(const int size = 10){
    List<Type>* res = (List<Type>*)calloc(1, sizeof(List<Type>));
    res->constructor(size);
    return res;
}

template<class Type>
void Delete(List<Type>* lst){
    lst->destructor();
    free(lst);
}
