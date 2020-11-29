/*******************************************************************************
* Filename: store_stage_info.c
*
* Description: This file contains the functions that create a new structure for
*   for each stage in the pipeline, stores all of the info in the correct place,
*   and prints it out.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

/* Local Header Files */
#include "store_stage_info.h"

/*------------------------------------------------------------------------------
* Function: malloc_struct_string
*
* Description: mallocs a buffer the length of string ( +1 for the null ),
*   copies the given string into the buffer and passes it back
*
* param: str - string to be transferred into a malloc'd buffer
* return: string stored in new buffer
*-----------------------------------------------------------------------------*/
char *malloc_struct_string( char *str ){
    char *loc = NULL;
    loc = malloc(strlen(str) + 1);
    if( loc == NULL ){
        perror("malloc struct string");
        exit( EXIT_FAILURE );
    }
    strcpy( loc, str );
    return loc;
}
/*------------------------------------------------------------------------------
* Function: store_stage_info
*
* Description: this function aims to store all of the useful information about
*   the stage into the structure. It determines what kind of input/output it
*   has and stores the command and arguments list.
*
* param: stage_pntr - a pointer to the structure that will contain all of the
*   info for that stage
* param: stage_num - which stage this is
* param: full_command - the string for the entire stage
* param: tokens - the list of separated strings that are in the full command,
*   this includes the redirection characters, if there are any
* param: num_tokens - the number of strings in the tokens array
* param: pipe_status - a variable that helps determine if this stage has an
*   input pipe, output pipe, both, or none
*-----------------------------------------------------------------------------*/
void store_stage_info( stage_t *stage_pntr, int stage_num, char *full_command,
        char *tokens[], int num_tokens, int pipe_status ){
    int i = 0;
    int args_cnt = 0; /* argc */
    /* First store the stage number in the struct */
    stage_pntr -> stage_num = stage_num;
    
    /* Store the full command */
    stage_pntr -> full_command = malloc_struct_string( full_command ); 
    
    /* First token should always be the executable, also store it in the
        argv list */
    stage_pntr -> command = malloc_struct_string( tokens[i] ); 
    (stage_pntr -> args_v)[args_cnt++] = malloc_struct_string( tokens[i++] );
    
    /* Now go through the remainder of the tokens and determine what are
        the inputs, outputs, and arguments */
    while( i < num_tokens ){
        /* if input redir symbol, take the next token is the input file */
        if( strcmp(tokens[i], "<") == 0 ){ 
            stage_pntr -> input = malloc_struct_string( tokens[++i] ); 
                /* Save the filename in the input variable */
            stage_pntr -> input_type = FILENAME;
                /* Set the type to file */
        }
        /* if output redir symbol, take the next token is the output file */
        else if( strcmp(tokens[i], ">") == 0 ){
            stage_pntr -> output = malloc_struct_string( tokens[++i] ); 
                /* Save the filename in the output variable */
            stage_pntr -> output_type = FILENAME;
                /* Set the type to file */
        }
        else{ /* Its an argument, so save in the args list, and add 1 to argc */
            stage_pntr -> args_v[args_cnt++] = malloc_struct_string( tokens[i]);
        }
        i++; /* Get the next token */
    }
    /* Once done with the tokens, args_cnt will be the total argc, store it */
    stage_pntr -> num_args = args_cnt;
    
    /* Check if the input/output is a pipe. Only set it if the type has not
        already been set to file above. If there is no pipe for the input/
        output, set it to stdin/stdout */
    switch( pipe_status ){
        case NO_PIPE:
            if( stage_pntr -> input_type == NOT_SET ){
                stage_pntr -> input_type = ORIG_STDIN;
            }
            if( stage_pntr -> output_type == NOT_SET ){
                stage_pntr -> output_type = ORIG_STDOUT;
            }
            break;
        case PIPE_OUT:
            stage_pntr -> output_type = PIPE_TO_STAGE;
            if( stage_pntr -> input_type == NOT_SET ){
                stage_pntr -> input_type = ORIG_STDIN;
            }
            break;
        case PIPE_IN:
            stage_pntr -> input_type = PIPE_FROM_STAGE;
            if( stage_pntr -> output_type == NOT_SET ){
                stage_pntr -> output_type = ORIG_STDOUT;
            }
            break;
        case PIPE_IN_OUT:
            stage_pntr -> input_type = PIPE_FROM_STAGE;
            stage_pntr -> output_type = PIPE_TO_STAGE;
            break;
        default:
            fprintf(stderr, "Could not determine pipe status.\n");
            break;
    }
}

/*------------------------------------------------------------------------------
* Function: create_stage_struct 
*
* Description: this function mallocs a new structure, initializes its values,
*   and returns a pointer to the new struct.
*
* return: a pointer to the new struct
*-----------------------------------------------------------------------------*/
stage_t *create_stage_struct( void ){
    stage_t *new_stage_pntr;

    new_stage_pntr = (stage_t *) malloc(sizeof(stage_t));
    if( !new_stage_pntr ){ /* Check return value */
        perror("malloc new stage struct");
        exit( EXIT_FAILURE );
    }
    
    /* Now Initialize the values */
    new_stage_pntr -> stage_num = 0;
    new_stage_pntr -> next_stage = NULL;
    new_stage_pntr -> input_type = 0;
    new_stage_pntr -> output_type = 0;
    new_stage_pntr -> full_command = NULL;
    new_stage_pntr -> command = NULL;
    new_stage_pntr -> input = NULL;
    new_stage_pntr -> output = NULL;
    new_stage_pntr -> num_args = 0;

    return new_stage_pntr;
}
#ifdef PRINTSTAGES
/*------------------------------------------------------------------------------
* Function: print_in_out
*
* Description: this function checks what kind of input and output type the
*   stage has and prints out the corresponding string
*
* param: stage - the struct to get the info from
*-----------------------------------------------------------------------------*/
void print_in_out( stage_t *stage ){
    
    printf("     input: ");
    switch( stage -> input_type ){
        case FILENAME:
            printf("%s\n", stage -> input );
            break;
        case ORIG_STDIN:
            printf("original stdin\n");
            break;
        case PIPE_FROM_STAGE:
            printf("pipe from stage %d\n", (stage -> stage_num) - 1 );
            break;
        case NOT_SET:
            fprintf(stderr, "Input type not set.\n");
            break;
    }
    
    printf("    output: ");
    switch( stage -> output_type ){
        case FILENAME:
            printf("%s\n", stage -> output );
            break;
        case ORIG_STDOUT:
            printf("original stdout\n");
            break;
        case PIPE_TO_STAGE:
            printf("pipe to stage %d\n", (stage -> stage_num) + 1 );
            break;
        case NOT_SET:
            fprintf(stderr, "Output type not set.\n");
            break;
    }
}
/*------------------------------------------------------------------------------
* Function: print_stages
*
* Description: This function simply prints out the information in each of the
*   stage structs in the linked list in the correct format.
*
* param: stage - the pointer to the first stage in the linked list
*-----------------------------------------------------------------------------*/
void print_stages( stage_t *stage ){
    int i;
    do{
        i = 0;
        printf("\n--------\nStage %d: \"%s\"\n--------\n", stage -> stage_num,
            stage -> full_command);
        print_in_out( stage );
        printf("      argc: %d\n", stage -> num_args );
        printf("      argv: ");
        while( i < (stage -> num_args) ){
            printf("\"%s\"", (stage -> args_v)[i] );
            i++;
            if( i != (stage -> num_args) ){
                printf(",");
            }
        }
        printf("\n");
    }while( (stage = stage -> next_stage) != NULL );
}
#endif
