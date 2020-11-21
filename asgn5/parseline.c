/*******************************************************************************
* Filename: parseline.c
*
* Description: 
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Local Header Files */
#include "check_valid_input.h"
#include "store_stage_info.h"

/* Defined Values */
#define NOT_SET 0
#define FILENAME 1
#define ORIG_STDIN 2
#define ORIG_STDOUT 2
#define PIPE_FROM_STAGE 3
#define PIPE_TO_STAGE 3

/* Structure */
    /* Will need stage #, pointer to next stage, stdin, or stdout, num args,
        command, args list, input_type, output_type */
struct stage{
    int stage_num;
    struct stage *next_stage;
    int input_type;
    int output_type;
    char *command;
    char *input;
    char *output;
    char *args_v[MAX_CMD_ARGS];
    int num_args;
    /*
    FILE *input_fd;
    FILE *output_fd;
    */
};
    
/*------------------------------------------------------------------------------
* Function: main 
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    char cmd[MAX_CMD_LENGTH];
    char *stages[MAX_STAGES];
    char *args_v[MAX_CMD_ARGS];
    int num_stages = 0;
    int pipe_set = 0;

    /* Read from stdin and check for overflow */
    read_into_buffer( cmd );     
    /* Check if the first character is a redirection or pipe */
    check_first_char( cmd[0] );
    /* Start parsing the lines by pipe */
    num_stages = parse_by_pipe( cmd, stages );
    printf("Stage Count: %d\n", num_stages );
    /* Now parse by spaces */
        /* Malloc and store the separate arguments into different strings */ 
    /* Malloc and initialize a structure for the stage */
    /* Parse through and store correct values into the structure */
    /* Set any flags to indicate the next stage is receiving a pipe */
    return 0;
}


