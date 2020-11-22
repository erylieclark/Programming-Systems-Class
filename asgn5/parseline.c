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
    char stage_buf[MAX_CMD_LENGTH];
    int num_stages = 0;
    int num_tokens = 0;
    int i = 0;
    int pipe_status = 0;
    stage_t *head_pntr;
    stage_t *cur_pntr;

#ifdef CHECK_INPUT
    /* Read from stdin and check for overflow */
    read_into_buffer( cmd );     
    /* Check if the first character is a redirection or pipe */
    check_first_char( cmd[0] );
    /* Start parsing the lines by pipe */
    num_stages = parse_by_pipe( cmd, stages );
    /* Now parse by spaces */
        /* Malloc and store the separate arguments into different strings */ 
    /* Malloc and initialize a structure for the stage */
    /* Parse through and store correct values into the structure */
    /* Set any flags to indicate the next stage is receiving a pipe */
#endif
    while( i < num_stages ){
        /* Before using strtok, store the complete stage into a separate buf */
        strcpy( stage_buf, stages[i] );
        /* Send each stage to be separated by spaces into different strings */
        num_tokens = parse_by_spaces( stages[i], stage_tokens );
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
        
        /* Now go through again and determine inputs, outputs, and arguments */
        if( i == 0 ){
            head_pntr = create_stage_struct();
            cur_pntr = head_pntr;
        }
        else{
            cur_pntr -> next_stage = create_stage_struct();
            cur_pntr = cur_pntr -> next_stage;
        }
        /* Place them in correct spots in a structure */
        store_stage_info( cur_pntr, i, stage_buf, stage_tokens, num_tokens,
            pipe_status); 

        i++;
    }
    print_stages( head_pntr );

    return 0;
}


