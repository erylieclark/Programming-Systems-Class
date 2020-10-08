#*******************************************************************************
# Filename: makefile
#
# Description: This makefile creates the executable in two steps:
#	1. First it check to see if any of the .c files are newer than the .o
# 		files, then compiles any that are updated.
#	2. Second, it links the .o files and creates the executable.
#
#*******************************************************************************

SHELL = /bin/bash
CC = gcc # Compile (and link, below) as C file
LD = gcc
CFLAGS = -Wall -g -ansi -pedantic # Flags used at compile time
TESTDIR = $(PWD)/MyTests
TESTINPUTS = $(TESTDIR)/*
TESTEXPECT = $(TESTDIR)/outputs/*
MAIN = fw
OBJ = $(MAIN).o user_input.o read_word.o hashing.o top_words.o
DEPS = user_input.h read_word.h hashing.h top_words.h

all: $(MAIN)

$(MAIN): $(OBJ) # Create uniq out of the listed object files
	$(LD) -o $@ $(OBJ) -I.

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

.PHONY: clean
clean: 
	rm -f $(OBJ) 

.PHONY: memcheck
memcheck: $(MAIN)
	valgrind -s --leak-check=yes --track-origins=yes ./$< < MyTests/dup_words

.PHONY: debug
debug: $(MAIN)
	gdb --args ./$< MyTests/empty MyTests/extra_spaces 

test: $(MAIN)
	@echo "Testing $(MAIN):"
	@cd $(TESTDIR); for input in $(TESTINPUTS); do 			\
	$(PWD)/$(MAIN) < $$input > $(MAIN).output;			\
	 fw < $$input > $(MAIN).expected;				\
	 if diff $(MAIN).output $(MAIN).expected > /dev/null ; then 	\
	  echo "ok.";				\
	 else					\
	  echo "FAILURE.";			\
	 fi;					\
	 rm -f $(MAIN).output $(MAIN).expected;	\
	done
