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
TESTDIR = /home/pn-cs357/lib/lab02/Inputs
TESTINPUTS = $(TESTDIR)/8_ShrinkingLines
TESTEXPECT = $(TESTDIR)/outputs/8_ShrinkingLines.expected
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

memcheck: test
	valgrind --leak-check=yes --track-origins=yes ./$<

debug: test
	gdb --args ./$< Inputs/1_OneLine

test: $(MAIN)
        @echo "Testing $(MAIN):"
	./$(MAIN) < $(TESTINPUTS) > $(MAIN).output
	if diff $(MAIN).output $(TESTEXPECT) > /dev/null ; then \
	 echo "ok.";
	else
	 echo "FAILURE.";
	fi;
	rm -f $(MAIN).output
	done
