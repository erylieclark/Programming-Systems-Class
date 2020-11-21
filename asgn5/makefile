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
MAIN = parseline
OBJ = parseline.o check_valid_input.o
DEPS = check_valid_input.h
all: $(MAIN)

$(MAIN): $(OBJ)  
	$(LD) -o $@ $^


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean: 
	rm -f $(OBJ)

.PHONY: memcheck
memcheck_enc: parseline
	valgrind -s --leak-check=yes --track-origins=yes ./$< < $(TESTDIR)/02_twochar

.PHONY: debug
debug: parseline
	gdb --args ./$< cvf tarfile Test

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
