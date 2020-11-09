/*******************************************************************************
* File: user_input.h
*
* Description: header file for the functions that primarily deal with the user
*   input. This includes reading optional input from the user, opening the
*   files, and determining if the inputs are valid.
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _USER_INPUT_H
#define _USER_INPUT_H

/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Global Variables */
extern int verb_list;
extern int strict;

/* Modes */
#define CREATE 0x01
#define LIST 0x02
#define EXTRACT 0x04
#define VERB 0x08
#define FARCH 0x10
#define STRICT 0x20
/* Input Arguments */
#define MIN_INPUTS 3
#define OPT_ARG 1
#define TAR_ARG 2
#define PATH_ARG 3
#define BLOCK_SIZE 512

void validate_options( char options );
int read_optional_input( int argc, char **argv );
FILE *open_file( char *argv, int mode );
char ** get_paths( int argc, char **argv, int mode );

#endif
