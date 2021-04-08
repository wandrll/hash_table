/**
 * \mainpage
 * \author Wandrl
 * \version 0.9
 * \date 2020.11.09
 * Double linked list. There ara pop, push, insert, erase functions.
 * Also there is function that order list by positions. If list ordered erase by position and get element by position works by O(1), otherwise it works by O(n)
 */
#pragma once
#include <stdio.h>
#define DEBUG_OFF

#ifdef DEBUG_ON
    #define IF_DEBUG_ON(code) code
#else
    #define IF_DEBUG_ON(code)
#endif
///\file
/**
 * Type of element  
 */

struct Pair{
    const char* key;
    const char* value;
};


typedef Pair list_elem;

struct Node{
    list_elem value;
    int next;
    int prev;
};

/**
 * Log data for dump  
 */
struct Log_data{
    const char* file;
    const char* func;
    int line;
};

Log_data* create_log_data(const char* file, const char* func, int line);


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

struct List{
    Node* data;
    size_t size;
    size_t capacity;
    size_t free;
    size_t head;
    size_t tail;
    bool is_ordered;
};

enum list_codes{
    LIST_OK,
    LIST_CONSTRUCTOR_ERROR,
    LIST_DESTRUCTOR_ERROR,
    LIST_INCREASE_ERROR,
    LIST_DECREASE_ERROR,
    LIST_EMPTY,
    LIST_WRONG_INDEX,
    LIST_ORDER_ERROR,
    LIST_OVERFLOW,
    LIST_UNDERFLOW,
    LIST_CORRUPTED
};
/**
 * Constructor
 * \param ls pointer on list
 */
list_codes list_constructor(List* ls);


List* New_List();




/**
 * Destructor
 * \param ls pointer on list
 */
list_codes list_destructor(List* ls);

void Delete_List(List* res);

/**
 * Add element in end of list
 * \param ls pointer on list
 * \param value element to push
 */
list_codes list_push_back(List* ls, list_elem value);

/**
 * Add element in begin of list
 * \param ls pointer on list
 * \param value element to push
 */
list_codes list_push_front(List* ls, list_elem value);

/**
 * Remove and return element from end of list
 * \param ls pointer on list
 * \param value pointer where to save element
 */
list_codes list_pop_back(List* ls, list_elem* value);


/**
 * Remove and return element from begin of list
 * \param ls pointer on list
 * \param value pointer where to save element
 */
list_codes list_pop_front(List* ls, list_elem* value);

/**
 * Get last element of list
 * \param ls pointer on list
 * \param value pointer where to save element
 */
list_codes list_back(List* ls, list_elem* value);

/**
 * Get first element of list
 * \param ls pointer on list
 * \param value pointer where to save element
 */
list_codes list_front(List* ls, list_elem* value);

/**
 * Linear search of index in array by position in list
 * \param ls pointer on list
 * \param num position
 * \param res pointer where to save index
 */
list_codes list_linear_find_index_by_position_if_isnt_ordered(List* ls, size_t num, size_t* res);

/**
 * Get next index by index
 * \param ls pointer on list
 * \param ind index
 * \param res pointer where to save next index
 */
list_codes list_get_next_index(List* ls, size_t ind, size_t* res);

/**
 * Get previous index by index
 * \param ls pointer on list
 * \param ind index
 * \param res pointer where to save previous index
 */
list_codes list_get_prev_index(List* ls, size_t ind, size_t* res);

/**
 * Get element by index
 * \param ls pointer on list
 * \param ind index
 * \param res pointer where to save value
 */
extern "C" list_codes list_get_value_by_index(List* ls, size_t ind, Pair* res);


/**
 * Get element by position
 * \param ls pointer on list
 * \param pos postion in list
 * \param res pointer where to save value
 */
list_codes list_get_value_by_position(List* ls, size_t pos, list_elem* res);

/**
 * Repalce element by index
 * \param ls pointer on list
 * \param ind index
 * \param val element that will replace
 */
list_codes list_replace_value_by_index(List* ls, size_t ind, list_elem val);

/**
 * Repalce element by position
 * \param ls pointer on list
 * \param pos position
 * \param val element that will replace
 */
list_codes list_replace_value_by_position(List* ls, size_t pos, list_elem val);

/**
 * Insert element after elemetn with index
 * \param ls pointer on list
 * \param value value that will insert
 * \param index index of element, after which the insert will be
 */
list_codes list_insert_by_index(List* ls, list_elem value, size_t index);

/**
 * Erase and save element with index
 * \param ls pointer on list
 * \param value pointer where to save element
 * \param index index of element, whihc must be erased
 */
list_codes list_erase_by_index(List* ls, list_elem* value, size_t index);

/**
 * Insert element after element with position
 * \param ls pointer on list
 * \param value value that will insert
 * \param position position of element, after which the insert will be
 */
list_codes list_insert_by_position(List* ls, list_elem value, size_t position);

/**
 * Erase and save element with index
 * \param ls pointer on list
 * \param value pointer where to save element
 * \param position position of element, whihc must be erased
 */
list_codes list_erase_by_position(List* ls, list_elem* value, size_t position);

/**
 * Order list by positions
 * \param ls pointer on list
 */
list_codes list_create_order(List* ls);

/**
 * Create awesome pdf file with list
 * \warning Supports only pdf format, be carefull
 * \warning Requirements: Graphviz with "circo"
 * \param ls pointer on list
 * \param file file where to save diagramm
 */
void list_dump_diag(List* ls, const char* file);

/**
 * Dump list(debug fuction)
 * \param ls pointer on list
 * \param file file where to dump
 */
void list_dump_file(List* ls, const char* file);

/**
 * Validate list
 * \param ls pointer on list
 * \param log log data to show where list was corrupted
 */
bool list_validation(List* ls, Log_data* log);
