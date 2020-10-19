/*******************************************************************************
* Filename: handle_input.h
*
* Description: This file contains the functions for checking valid user input
*    and opening the files.
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _HANDLE_INPUT_H
#define _HANDLE_INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Global Variables for passing around the file descriptors */
extern int readfd;
extern int writefd;

/* How many inputs did the user give */
#define MAX_INPUTS 3
#define MIN_INPUTS 2
#define NO_INPUTS 1

/* Permissions when opening files */
#define READ_F 0
#define WRITE_F 1
#define RDWRT_F 2

/* Is this supposed to be an input or output file */
#define INPUT_F 1
#define OUTPUT_F 2

void parse_input_encode( int argc, char **argv );
void parse_input_decode( int argc, char **argv );
int open_file( char *argv, int mode );

#endif /*_HANDLE_INPUT_H */
