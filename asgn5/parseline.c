/*******************************************************************************
* Filename: parseline.c
*
* Description: parseline is used to take the input from a command line and
*   separate out the stages of a pipeline, and determine where all of the inputs
*   and outputs are.
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

/*------------------------------------------------------------------------------
* Function: main 
*
* Description: main directs all of the other functions to perform in the 
*   correct order. The main steps it takes are to read the command line into
*   a buffer, separate the command line by stages(between pipes), check for
*   erros with this, separate out each stage into separate strings, check for
*   errors with this as well, and store all of the info into a struct and
*   print all of it out.
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    char cmd[MAX_CMD_LENGTH]; /* the full command line string */
    char *stages[MAX_STAGES]; /* pointers to each stage string */
    char *stage_tokens[MAX_STAGE_TOKENS + 1]; /* One for Null at end */ 
    char stage_buf[MAX_CMD_LENGTH];
    int num_stages = 0;
    int num_tokens = 0;
    int i = 0;
    int pipe_status = 0;
    stage_t *head_pntr;
    stage_t *cur_pntr;

    /* Read from stdin and check for overflow */
    read_into_buffer( cmd );     
    /* Check if the first character is a redirection or pipe, error if yes */
    check_first_char( cmd[0] );
    /* Start parsing the lines by pipe */
    num_stages = parse_by_pipe( cmd, stages );
    
    while( i < num_stages ){

        /* Before using strtok, store the complete stage into a separate buf,
            in case strtok modifies it */
        strcpy( stage_buf, stages[i] );

        /* Send each stage to be separated by spaces into different strings */
        num_tokens = parse_by_spaces( stages[i], stage_tokens );

        /* Determine where in the pipeline this stage is */
        if( i == 0 && num_stages <= (i + 1) ){ /* If only one stage */
            pipe_status = NO_PIPE; /* This stage does not have any pipes */
        }
        else if( i == 0 && num_stages > (i + 1) ){ /* First stage */
            pipe_status = PIPE_OUT; /* Only has a pipe out */
        }
        else if( i > 0 && num_stages > (i + 1) ) { /* Not the first, but
            not the last */ 
            pipe_status = PIPE_IN_OUT; /* Has pipe in and out */
        }
        else if( i > 0 && num_stages == (i + 1) ){ /* Last Stage */
            pipe_status = PIPE_IN; /* Only has pipe in */
        }
        else{
            fprintf(stderr, "Could not determine location in pipeline.\n");
        }

        /* Now check that redirections are appropriate for spot in pipeline */
        check_redirs( stage_tokens, num_tokens, pipe_status );
        
        /* Now go through again and determine inputs, outputs, and arguments */
        if( i == 0 ){ /* For the first stage, set head pnter to the struct */
            head_pntr = create_stage_struct();
            cur_pntr = head_pntr; 
        }
        else{
            /* Store a pointer to this new struct into the previous struct to
                continue the linked list */
            cur_pntr -> next_stage = create_stage_struct();
            /* Now set cur to the new pointer */
            cur_pntr = cur_pntr -> next_stage;
        }
        /* Get in the info and store it in the structure */
        store_stage_info( cur_pntr, i, stage_buf, stage_tokens, num_tokens,
            pipe_status); 

        i++; /* Next stage */
    }
    
    /* Give the head of the linked list to the print function */
    print_stages( head_pntr );

    return 0;
}


