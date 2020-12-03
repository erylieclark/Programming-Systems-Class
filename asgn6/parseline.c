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

#include "parseline.h"

stage_t *head_pntr; /* Global variable */

/*------------------------------------------------------------------------------
* Function: free_stages
*
* Description: main directs all of the other functions to perform in the 
* 
* return: 0 on success, -1 on failure if no malloc'd memory, -2 on
*-----------------------------------------------------------------------------*/
void free_tokens( char *stage_tokens[], int num_tokens ){
    while( num_tokens ){ /* Free the tokens associated with the stage */
        free( stage_tokens[--num_tokens] );
    }
}
/*------------------------------------------------------------------------------
* Function: free_stages 
*
* Description: main directs all of the other functions to perform in the 
* 
* return: 0 on success, -1 on failure if no malloc'd memory, -2 on
*-----------------------------------------------------------------------------*/
void free_stages( char *stages[], int num_stages, int cur_stage){
    while( cur_stage != num_stages ){ /* Free all of the remaining stages */
        free( stages[cur_stage++] );
    }
}
/*------------------------------------------------------------------------------
* Function: parseline 
*
* Description: main directs all of the other functions to perform in the 
*   correct order. The main steps it takes are to read the command line into
*   a buffer, separate the command line by stages(between pipes), check for
*   erros with this, separate out each stage into separate strings, check for
*   errors with this as well, and store all of the info into a struct and
*   print all of it out.
* 
* return: 0 on success, -1 on failure 
*-----------------------------------------------------------------------------*/
int parseline( void ){
    char cmd[MAX_CMD_LENGTH]; /* the full command line string */
    char *stages[MAX_STAGES + 1]; /* pointers to each stage string */
    char *stage_tokens[MAX_STAGE_TOKENS + 1]; /* One for Null at end */ 
    char stage_buf[MAX_CMD_LENGTH];
    int num_stages = 0;
    int num_tokens = 0;
    int res, i = 0;
    int pipe_status = 0;
    stage_t *cur_pntr;
    head_pntr = NULL; /* Init head pointer */

    /* Read from stdin and check for overflow */
    if( read_into_buffer( cmd ) ){ /* -1 on error, 0 on success */
       return -1; /* No allocated memory yet */
    }
    /* Check if the first character is a redirection or pipe, error if yes */
    if( check_first_char( cmd[0] ) ){ /* -1 on error, 0 on success */
        return -1; /* No allocated memory yet */
    }
    /* Start parsing the lines by pipe */
    num_stages = parse_by_pipe( cmd, stages );
    if( num_stages == -1 ){ /* -1 on failure, num stages on success */
        return -1;
    }
    while( i < num_stages ){

        /* Before using strtok, store the complete stage into a separate buf,
            in case strtok modifies it */
        strcpy( stage_buf, stages[i] );

        /* Send each stage to be separated by spaces into different strings */
        num_tokens = parse_by_spaces( stages[i], stage_tokens );
        if( num_tokens == -1 ){ /* -1 on failure, num tokens on success */
            free_stages( stages, num_stages, i );
            cleanup();
            return -1; /* all allocated memory has been freed */
        }
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
        if( check_redirs( stage_tokens, num_tokens, pipe_status ) ){
            free_tokens( stage_tokens, num_tokens );
            free_stages( stages, num_stages, i);
            cleanup();
            return -1;
        }
        /* Now go through again and determine inputs, outputs, and arguments */
        if( i == 0 ){ /* For the first stage, set head pnter to the struct */
            head_pntr = create_stage_struct();
            if( head_pntr == NULL ){ /* malloc failed */
                free_tokens( stage_tokens, num_tokens );
                free_stages( stages, num_stages, i);
                    /* No cleanup here becasue head_pntr failed */
                return -1;
            }
            cur_pntr = head_pntr; 
        }
        else{
            /* Store a pointer to this new struct into the previous struct to
                continue the linked list */
            cur_pntr -> next_stage = create_stage_struct();
            if( cur_pntr -> next_stage == NULL ){ /* malloc failed */
                free_tokens( stage_tokens, num_tokens );
                free_stages( stages, num_stages, i);
                cleanup();
                return -1;
            }
            /* Now set cur to the new pointer */
            cur_pntr = cur_pntr -> next_stage;
        }
        /* Get the info and store it in the structure */
        res = store_stage_info( cur_pntr, i, stage_buf, stage_tokens,
            num_tokens, pipe_status); 
        if( res == -1 ){
            free_tokens( stage_tokens, num_tokens ); /* free the tokens */
            free_stages( stages, num_stages, i );
            cleanup();
            return -1;
        }
        free( stages[i] ); /* Free the stage after storing it */
        free_tokens( stage_tokens, num_tokens ); /* free the tokens */
        i++; /* Next stage */
    }

    /* Handle cd here */
    if( num_stages == 1){ /* if its only one stage, check it for cd */
        if( strcmp("cd", (head_pntr -> args_v)[0] ) == 0 ){
            if( head_pntr -> num_args < 2 ){ /* If no arg, cd to home dir */
                if( chdir("/") ){
                    perror("cannot cd to home directory");
                    cleanup();
                    return -1;
                }
            }
            else if( head_pntr -> num_args == 2 ){ /* if one arg, cd to it */
                if( chdir( (head_pntr -> args_v)[1] ) ){
                    perror( (head_pntr -> args_v)[1] );
                    cleanup();
                    return -1;
                }
            }
            else if( head_pntr -> num_args > 2 ){ /* too many args */
                fprintf(stderr, "cd: Too many arguments\n");
                cleanup();
                return -1;
            }
            /* If cd is successful, cleanup after it */
            cleanup();
        }
    }
    return 0;
}
