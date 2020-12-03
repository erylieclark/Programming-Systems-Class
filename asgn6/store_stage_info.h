/*******************************************************************************
* File: store_stage_info.h
*
* Description: This file contains the functions that create a new structure for
*   for each stage in the pipeline, stores all of the info in the correct place,
*   and prints it out.
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _STORE_STAGE_INFO_H 
#define _STORE_STAGE_INFO_H

/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Local Header Files */
#include "check_valid_input.h"

/* Defined Values */
#define NOT_SET 0
#define FILENAME 1
#define ORIG_STDIN 2
#define ORIG_STDOUT 2
#define PIPE_FROM_STAGE 3
#define PIPE_TO_STAGE 3

/* Structure */
struct stage{
    int stage_num;
    struct stage *next_stage;
    int input_type;
    int output_type;
    char *full_command;
    char *command;
    char *input;
    char *output;
    char *args_v[MAX_CMD_ARGS + 1]; /* One for terminating NULL */
    int num_args;
};
typedef struct stage stage_t;

char *malloc_struct_string( char *str );
int store_stage_info( stage_t *stage_pntr, int stage_num, char *full_command,
    char *tokens[], int num_tokens, int pipe_status );
stage_t *create_stage_struct( void );

#endif /* _STORE_STAGE_INFO_H */

