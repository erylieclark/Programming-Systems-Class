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
TESTDIR = $(PWD)/Inputs
TESTINPUTS = $(TESTDIR)/*
TESTEXPECT = $(TESTDIR)/outputs/*
MAIN = uniq
OBJ = $(MAIN).o

all: $(MAIN)

$(MAIN): $(OBJ) # Create uniq out of the listed object files
	$(LD) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean: 
	rm -f $(OBJ)

.PHONY: memcheck
memcheck: $(MAIN)
	valgrind -s --leak-check=yes --track-origins=yes ./$< < $(TESTDIR)/2_TwoDifferentLines

.PHONY: debug
debug: $(MAIN)
	gdb --args ./$< Inputs/2_TwoDifferentLines

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
