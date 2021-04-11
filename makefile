CFLAGS= -c  -O0 
LDFLAGS=  -O0 
TMP = -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr
CTEMP=-Wall -Werror -Wextra -pedantic -Wshadow -Wconversion -Wsign-conversion
all: dict

dict: main.o list.o hash_table.o dictionary.o get_value.o hash_table.hpp list.hpp dictionary.hpp
	g++ $(LDFLAGS) main.o list.o hash_table.o dictionary.o get_value.o -o dict 

main.o: main.cpp hash_table.hpp
	g++ $(CFLAGS) main.cpp -msse4

list.o: list.cpp list.hpp
	g++ $(CFLAGS) list.cpp

hash_table.o: hash_table.cpp hash_table.hpp
	g++ $(CFLAGS) hash_table.cpp -masm=intel -msse4

dictionary.o: dictionary.cpp dictionary.hpp
	g++ $(CFLAGS) dictionary.cpp

get_value.o: get_value.nas
	nasm -f elf64 get_value.nas


clean:
	rm -rf *.o 