# Accelerated Hash map
First of all, it is an educational project for the second term of MIPT. Secondly, excuse me for my english 
## Requirments
1. CPU with _mm_crc32_u64 instruction compatibility
1. NASM (The Netwide Assembler)
## Small addition
Since this is an educational project, i usen zero opitimisation level (-O0 flag in gcc), otherwise the difficulty of the task increases significantly.
## First step
To start, i have written simple and small hash map (const char*, const char*), which supports insert and get operations. Then, i have added external interface, which allows load dictionary from file, based on this it builds hash map, and alos there is method to get definition of a word. I have decided, that once you load dictionary, you will not remove elements from it, maybe only add, so i measure time of getting definition of the word. In tests i get defenition of every word in dictionary 1000 times. I have run my programm with Intel VTune Profiler and recieved some inforamtion about perfomance
![Image alt](https://github.com/wandrll/hash_table/raw/master/readme_src/before.jpg)
As we can see, most of the time is spent on functions get, hash, __strcmp_avx2, list_get_value_by_index. Let's consider them in more detail
1. get - It is function of hash map, which return value by key if this key is in table
2. hash - This function calculate hash of string line
3. __strcmp_avx2 - It is standart strcmp fucntion, and unfortunatly i can't improve its perfomance
4. list_get_value_by_index - Function of my [Double linked list](https://github.com/wandrll/list).
## Optimizations
### hash
I have decided to start with the easiest function to optimize. This function was pretty simple and was passisg all characters one by one and calculated hash.

```c++
static unsigned long long hash(const char* line){
    
    unsigned long long hash = 5381;
  
    while (*line){
        hash = ((hash << 5) + hash) + *line;
        line++;
    }

    return hash;
 
}

```

The main opimization here is calculate hash not for only for one symbol, but for 8. Unfortunatly, this affects on input format, so now all memory allocated on lines must be alligned at 8 bytes, and last 8 bytes equal to 0. For example correct line will be "somethg"000000000. This optimization accelerate hash function in only about 5 times, because there are a lot of words in dictionry, which lengths is less than 8 bytes, but anyway, it is good increase of perfomance. 
Now this function looks like this:
```c++
extern "C" unsigned long long hash_64(const char* line){
    
    unsigned long long hash = 0;

    while (*((unsigned long long*)line)){
        hash = _mm_crc32_u64(hash, *((unsigned long long*)line));
        line+=8;
    }

    return hash;

}

```
I have declared this function as extern "C", becuase later i will call it in assembler function
### get && list_get_value_by_index
First of all, function list_get_value_by_index uses only in function get. Secondly, the main problem in perfomance here is a lot of jumps by pointers in different memory areas, so i have decided to rewrite this function on assmebler, in order to reduce as much as i can memory access operations. 
```c++
bool get(Hash_table* ths, const char* key, const char** result){

    size_t index = hash_64(key)%(ths->bucket_count);

    List* ptr = ths->data[index]; 

    if(!ptr){
        return false;
    }

    size_t count = ptr->size;


    Pair curr = {};

    for(size_t i = 1; i <= count; i++){
        list_get_value_by_index(ptr, i, &curr);

        if(strcmp(key, curr.key) == 0){
            *result = curr.value;
    
            return true;
        }
    }

    return false;
}
```
You can check objump of get function [here](readme_src/not_opt_dump.txt). It looks pretty ugly and contains a lot of memory access operations, so i have rewritten this function on [assembler](get_value.nas). Instead of using function list_get_value_by_index, i use knowledge about internal structure of my list, so i calculate address only once, and after that i just move pointer by increasing. Also, new function returns value, instead of flag, and becuase value is const char*, if there is no element with key, this function returns NULL

## Conclusion
Let's run this programm in VTune Profiler again.
![Image alt](https://github.com/wandrll/hash_table/raw/master/readme_src/after.jpg)
As you can see, perfomance have been massivly increased. Total acceleration is about 1.5 times.

P.S. If i build first version with -O1 flag, i get the same results which i get with accelerated version.


