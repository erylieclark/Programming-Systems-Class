/*******************************************************************************
* File: user_input.h
*
* Description: header file for the functions that primarily deal with the user
*   input. This includes reading optional input from the user and opening the
*   files or stdin and returning a pointer.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#ifndef _USER_INPUT_H
#define _USER_INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>


#define DEFAULT_NUM_WORDS 10

int read_optional_input(int argc, char **argv); 
FILE *open_file(int argc, char **argv);

#endif
