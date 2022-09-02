# Executable
BINFOLDER := bin/
# .hpp
INCFOLDER := inc/
# .cpp
SRCFOLDER := src/
# .o
OBJFOLDER := obj/
CC := gcc
CXX := g++
CPPFLAGS := -W -Wall -ansi -std=c++11 -lwiringPi -lpthread -pedantic -ggdb 
CFLAGS := -W -Wall -lwiringPi -ggdb 
LDFLAGS=-ggdb
LDLIBS=-lwiringPi -lpthread
CXX_SRCFILES := $(wildcard src/*.cpp)
C_SRCFILES := $(wildcard src/*.c)
CXX_OBJECTS = $(patsubst src/%.cpp,obj/%.o,$(CXX_SRCFILES))
C_OBJECTS = $(patsubst src/%.c,obj/%.o,$(C_SRCFILES))
all: $(CXX_OBJECTS) $(C_OBJECTS)
	@ echo 'Construindo arquivo binario usando GCC linker: $<'
	$(CXX) $(LDFLAGS) obj/*.o -o bin/prog $(LDLIBS)
	@ echo 'Terminou a construção do binario: bin/prog'
	@ echo ' '
	
obj/%.o: src/%.cpp
	@ echo 'Construindo target usando GCC compiler: $<'
	$(CXX) $(CPPFLAGS) -c $< -o $@ -I./inc
	@ echo ' '

obj/%.o: src/%.c
	@ echo 'Construindo target usando GCC compiler: $<'
	$(CC) $(CFLAGS) -c $< -o $@ -I./inc
	@ echo ' '

run: bin/prog
	bin/prog
.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf bin/*
