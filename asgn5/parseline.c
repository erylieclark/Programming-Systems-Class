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

#define CHECK_INPUT
/*
#define TEST_PRINT
*/

/*------------------------------------------------------------------------------
* Function: main 
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    char cmd[MAX_CMD_LENGTH];
    char *stages[MAX_STAGES];
    char *stage_tokens[MAX_STAGE_TOKENS + 1]; /* One for Null at end */ 
    int num_stages = 0;
    int num_tokens = 0;
    int i = 0;
    int pipe_status = 0;
    stage_t s0,s1,s2,s3,s4;
#ifdef CHECK_INPUT
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
#endif
    while( i < num_stages ){

        /* Send each stage to be separated by spaces into different strings */
        num_tokens = parse_by_spaces( stages[i], stage_tokens );
        printf("Num Tokens: %d, i = %d\n", num_tokens, i );  
        /* Determine where in the pipeline this stage is */
        if( i == 0 && num_stages <= (i + 1) ){ /* If only one stage */
            pipe_status = NO_PIPE; /* This stage does not have any pipes */
        }
        else if( i == 0 && num_stages > (i + 1) ){ /* First stage */
            pipe_status = PIPE_OUT;
        }
        else if( i > 0 && num_stages > (i + 1) ) { /* Not the first, but
            not the last */ 
            pipe_status = PIPE_IN_OUT;
        }
        else if( i > 0 && num_stages == (i + 1) ){ /* Last Stage */
            pipe_status = PIPE_IN;
        }
        else{
            fprintf(stderr, "Could not determine location in pipeline.\n");
        }

        /* Now check that redirections are appropriate for spot in pipeline */
        check_redirs( stage_tokens, num_tokens, pipe_status );
        /* Check the number of each type of argument in each stage */
        /* Make sure it doesn't have more than one < or > */
        /* If there is a next stage, make sure is doesn't have a > */
        /* If there is a previous stage, make sure it doesnt have a < */
        /* Now go through again and determine inputs, outputs, and arguments */
        /* Place them in correct spots in a structure */
    

        i++;
    }


#ifdef TEST_PRINT
    /* Print out all the stages */
    s0.stage_num = 0;
    s0.next_stage = &s1;
    s0.input_type = FILENAME;
    s0.output_type = PIPE_TO_STAGE;
    s0.full_command = "ls < one";
    s0.command = "ls";
    s0.input = "one";
    s0.output = NULL;
    s0.args_v[0] = "ls";
    /* s0.args_v[1] = NULL; */
    s0.num_args = 1;

    s1.stage_num = 1;
    s1.next_stage = &s2;
    s1.input_type = PIPE_FROM_STAGE;
    s1.output_type = PIPE_TO_STAGE;
    s1.full_command = "more";
    s1.command = "more";
    s1.input = NULL;
    s1.output = NULL;
    s1.args_v[0] = "more";
    /* s0.args_v[1] = NULL; */
    s1.num_args = 1;
    
    s2.stage_num = 2;
    s2.next_stage = NULL;
    s2.input_type = PIPE_FROM_STAGE;
    s2.output_type = ORIG_STDOUT;
    s2.full_command = "sort";
    s2.command = "sort";
    s2.input = NULL;
    s2.output = NULL;
    s2.args_v[0] = "sort";
    /* s0.args_v[1] = NULL; */
    s2.num_args = 1;
    
    print_stages( &s0 );
#endif
    return 0;
}


