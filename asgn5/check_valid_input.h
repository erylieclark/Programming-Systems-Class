/*******************************************************************************
* File: check_valid_input.h
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _CHECK_VALID_INPUT_H
#define _CHECK_VALID_INPUT_H


/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Global Variables */

#define MAX_CMD_LENGTH (512 + 2) /* One for \n and oen for \0 */
#define MAX_STAGES 10
#define MAX_CMD_ARGS 10
#define PIPE_DELIM "|"
#define SPACE_DELIM " "

int parse_by_pipe( char cmd[], char *stages[] );
void check_first_char( char c );
void read_into_buffer( char cmd[] );
void store_stage( int loc, char *t, char *stages[] );
void check_for_empty_stage( char *tok );

#endif /* _CHECK_VALID_INPUT_H */

