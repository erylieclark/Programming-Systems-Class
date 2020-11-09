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
CFLAGS = -Wall -g -pedantic # Flags used at compile time
TESTDIR = $(PWD)/inputs
TESTINPUTS = $(TESTDIR)/*
TESTEXPECT = $(TESTDIR)/outputs/*
MAIN = mytar 
OBJ = mytar.o user_input.o create_tar.o create_header.o read_write.o read_tar.o read_header.o 
DEPS = user_input.h create_tar.h create_header.h read_write.h read_tar.h read_header.h
all: $(MAIN)

mytar: mytar.o $(OBJ) # Create uniq out of the listed object files
	$(LD) -o $@ $^


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean: 
	rm -f $(OBJ)

.PHONY: memcheck
memcheck_enc: mytar
	valgrind -s --leak-check=yes --track-origins=yes ./$< < $(TESTDIR)/02_twochar

.PHONY: debug
debug: mytar 
	gdb --args ./$< xf tarfile.exp

test: $(MAIN)
	@echo "Testing $(MAIN):"
	@cd $(TESTDIR); for input in $(TESTINPUTS); do 			\
	$(PWD)/$(MAIN) < $$input > $(MAIN).output;			\
	 uniq < $$input > $(MAIN).expected;				\
	 if diff $(MAIN).output $(MAIN).expected > /dev/null ; then 	\
	  echo "ok.";				\
	 else					\
	  echo "FAILURE.";			\
	 fi;					\
	 rm -f $(MAIN).output $(MAIN).expected;	\
	done
