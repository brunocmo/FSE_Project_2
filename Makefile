# Executable
BINFOLDER := bin/
# .hpp
INCFOLDER := inc/
# .cpp
SRCFOLDER := src/
# .o
OBJFOLDER := obj/
CC := g++
CPPFLAGS :=-W -Wall -ansi -std=c++11 -lwiringPi -lpthread -pedantic -ggdb 
LDFLAGS=-ggdb
LDLIBS=-lwiringPi -lpthread
CFLAGS := -W -Wall -ggdb -x

SRCFILES := $(wildcard src/*.cpp)
all: $(SRCFILES:src/%.cpp=obj/%.o)
	@ echo 'Construindo arquivo binario usando GCC linker: $<'
	$(CC) $(LDFLAGS) obj/*.o -o bin/prog $(LDLIBS)
	@ echo 'Terminou a construção do binario: bin/prog'
	@ echo ' '

obj/%.o: src/%.cpp
	@ echo 'Construindo target usando GCC compiler: $<'
	$(CC) $(CPPFLAGS) -c $< -o $@ -I./inc
	@ echo ' '

run: bin/prog
	bin/prog
.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf bin/*
