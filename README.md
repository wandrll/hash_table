# Accelerated Hash map
First of all, it is an educational project for the second term of MIPT. Secondly, excuse me for my english 
## Requirments:
1. CPU with _mm_crc32_u64 instruction compatibility
1. NASM (The Netwide Assembler)

## First step
To start, i have written simple and small hash map (const char*, const char*), which supports insert and get operations. Then, i have added external interface, which allows load dictionary from file, based on this it builds hash map, and then there is method to get definition of a word. I have decided, that once you load dictionary, you will not remove elements from it, maybe only add, so i measure time of getting definition of the word. In tests i get defenition of every word in dictionary 1000 times. I have run my programm with Intel VTune Profiler and recieved some inforamtion about perfomance
![Image alt](https://github.com/wandrll/hash_table/raw/master/images/before.jpg)
As we see, most of the time is spent on functions get, hash, __strcmp_avx2, list_get_value_by_index. Let's consider them in more detail
1. get - It is function of hash map, which return value by key if this key is in table
2. hash - This function hash string line
3. __strcmp_avx2 - It is standart strcmp fucntion, and unfortunatly i can't improve perfomnce of it
4. list_get_value_by_index - Fucntion of my [Double linked list](https://github.com/wandrll/list).
## Optimizations
### hash
I have decided to start with the easiest function to optimize. This function was pretty stright and simply pass all characters one by one and calculated hash.


## Conclusion
The final acceleration of the program is from three to four times
