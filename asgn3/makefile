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
TESTDIR = $(PWD)/inputs
TESTINPUTS = $(TESTDIR)/*
TESTEXPECT = $(TESTDIR)/outputs/*
MAIN = hencode hdecode
OBJ = handle_input.o read_write.o hist.o huff_tree.o\
 write_huff.o read_huff.o
DEPS = handle_input.h read_write.h hist.h huff_tree.h write_huff.h read_huff.h
all: $(MAIN)

hencode: hencode.o $(OBJ) # Create uniq out of the listed object files
	$(LD) -o $@ $^

hdecode: hdecode.o $(OBJ) # Create uniq out of the listed object files
	$(LD) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean: 
	rm -f $(OBJ)

.PHONY: memcheck_enc
memcheck_enc: hencode
	valgrind -s --leak-check=yes --track-origins=yes ./$< < $(TESTDIR)/02_twochar

.PHONY: memcheck_dec
memcheck_dec: hdecode
	valgrind -s --leak-check=yes --track-origins=yes ./$< < $(TESTDIR)/02_twochar

.PHONY: debug_enc
debug_enc: hencode
	gdb --args ./$< inputs/09_sample outfile

.PHONY: debug_dec
debug_dec: hdecode
	gdb --args ./$< outfile outfile.dec

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
