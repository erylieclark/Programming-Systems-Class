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
* Function: print_in_out
*-----------------------------------------------------------------------------*/


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






