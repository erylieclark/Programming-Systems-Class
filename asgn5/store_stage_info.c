/*******************************************************************************
* Filename: store_stage_info.c
*
* Description: 
*
* Author: Erin Rylie Clark
*
*******************************************************************************/


/* Local Header Files */
#include "store_stage_info.h"

/*------------------------------------------------------------------------------
* Function: malloc_struct_string
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
*-----------------------------------------------------------------------------*/
void store_stage_info( stage_t *stage_pntr, int stage_num, char *full_command,
        char *tokens[], int num_tokens, int pipe_status ){
    int i = 0;
    int args_cnt = 0;
    
    stage_pntr -> stage_num = stage_num;
    
    stage_pntr -> full_command = malloc_struct_string( full_command ); 
    /* First token should always be the command */
    stage_pntr -> command = malloc_struct_string( tokens[i] ); 
    (stage_pntr -> args_v)[args_cnt++] = malloc_struct_string( tokens[i++] );

    while( i < num_tokens ){
        if( strcmp(tokens[i], "<") == 0 ){
            stage_pntr -> input = malloc_struct_string( tokens[++i] ); 
            stage_pntr -> input_type = FILENAME;
        }
        else if( strcmp(tokens[i], ">") == 0 ){
            stage_pntr -> output = malloc_struct_string( tokens[++i] ); 
            stage_pntr -> input_type = FILENAME;
        }
        else{ /* Its an argument */
            stage_pntr -> args_v[args_cnt++] = malloc_struct_string( tokens[i]);
        }
        i++;
    }
    stage_pntr -> num_args = args_cnt;

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
/*------------------------------------------------------------------------------
* Function: print_in_out
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






