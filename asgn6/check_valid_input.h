/*******************************************************************************
* File: check_valid_input.h
*
* Description: these functions mainly deal with the separating out of the
*   command line into information that can be more easily stored later on. They
*   also do the majority of the usage error checking 
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
#include <signal.h>

#define MAX_CMD_LENGTH (512 + 2) /* One for \n and oen for \0 */
#define MAX_STAGES 10
#define MAX_CMD_ARGS 10
#define MAX_STAGE_TOKENS (MAX_CMD_ARGS + 5)
#define PIPE_DELIM "|"
#define SPACE_DELIM " "
#define NO_PIPE 0
#define PIPE_IN 1
#define PIPE_OUT 2
#define PIPE_IN_OUT 3

int check_redirs( char *stage_tokens[], int num_tokens, int pipe_status );
int parse_by_spaces( char *stage, char *stage_tokens[] );
char *trim_trailing_space( char *str );
int parse_by_pipe( char cmd[], char *stages[] );
int check_first_char( char c );
int read_into_buffer( char cmd[] );
int store_string( int loc, char *t, char *stages[] );
int check_for_empty_stage( char *tok );

#endif /* _CHECK_VALID_INPUT_H */

