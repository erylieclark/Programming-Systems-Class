#*******************************************************************************
# Filename: makefile
#
# Description: This makefile creates the executable in two steps:
#	1. First it check to see if any of the .c files are newer than the .o
# 		files, then compiles any that are updated.
#	2. Second, it links the .o files and creates the executable.
#
#*******************************************************************************

CC = gcc # Compile (and link, below) as C file
LD = gcc
CFLAGS = -Wall -g -ansi -pedantic # Flags used at compile time
LDFLAGS = 
DEPS = 
OBJ = uniq.o

uniq: $(OBJ) # Create uniq out of the listed object files
	$(LD) -o $@ $< # Link the object files

test: test.o
	$(LD) -o $@ $< # Link the object files

%.o: %.c $(DEPS) # All .o files depend on .c files and dependencies listed
	$(CC) $(CFLAGS) -c -o $@ $< # Compile the .c files

.PHONY: clean
clean: 
	rm -f $(OBJ)
	echo Clean done

memcheck: test
	valgrind --leak-check=yes --track-origins=yes ./$<

debug: test
	gdb --args ./$< Inputs/1_OneLine
