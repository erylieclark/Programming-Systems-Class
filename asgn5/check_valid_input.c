/*******************************************************************************
* Filename: check_valid_input.c
*
* Description: 
*
* Author: Erin Rylie Clark
*
*******************************************************************************/


/* Local Header Files */
#include "check_valid_input.h"

/*------------------------------------------------------------------------------
* Function: parse_by_spaces
*-----------------------------------------------------------------------------*/
int parse_by_spaces( char *stage ){
    int i = 0;
    char *token;

    token = strtok( cmd, PIPE_DELIM );
    do{
        check_for_empty_stage( token );
        store_stage( i, token, stages );
        printf("%02d) <%s>\n", i++, token);
        if( i > MAX_STAGES ){
            fprintf(stderr, "pipeline too deep\n");
            exit( EXIT_FAILURE );
        }
    }while( (token = strtok( NULL, PIPE_DELIM )) );

    return i; /* Number of stages */
}
/*------------------------------------------------------------------------------
* Function: parse_by_pipe
*-----------------------------------------------------------------------------*/
int parse_by_pipe( char cmd[], char *stages[] ){
    int i = 0;
    char *token;

    token = strtok( cmd, PIPE_DELIM );
    do{
        check_for_empty_stage( token );
        store_stage( i, token, stages );
        printf("%02d) <%s>\n", i++, token);
        if( i > MAX_STAGES ){
            fprintf(stderr, "pipeline too deep\n");
            exit( EXIT_FAILURE );
        }
    }while( (token = strtok( NULL, PIPE_DELIM )) );

    return i; /* Number of stages */
}
/*------------------------------------------------------------------------------
* Function: check_first_char 
*-----------------------------------------------------------------------------*/
void check_first_char( char c ){

    /* Check that first char isn't a redirection or pipe */
    if( c == '|' ){ 
        fprintf(stderr, "no command given\n");
        exit( EXIT_FAILURE );
    }
    else if( c == '<' ){
        fprintf(stderr, "bad input redirection\n");
        exit( EXIT_FAILURE );
    
    }
    else if( c == '>' ){
        fprintf(stderr, "bad output redirection\n");
        exit( EXIT_FAILURE );
    }
}

/*------------------------------------------------------------------------------
* Function: read_into_buffer 
*-----------------------------------------------------------------------------*/
void read_into_buffer( char cmd[] ){

    /* Read from stdin and store into a buffer */
    if( (fgets( cmd, MAX_CMD_LENGTH, stdin )) == NULL ){
        if( feof(stdin) ){
            printf("Nothing in stdin\n");
        }
        else{
            perror("fgets");
            exit( EXIT_FAILURE );
        }
    }
    /* Check that it does not overflow the buffer */
    else if( strchr( cmd, '\n' ) == NULL ){
        fprintf(stderr, "command too long\n");
        fflush(stdin); /* Get rid of the remaining input if any */
            /* Will need to not exit here in mush? */
        exit( EXIT_FAILURE );
    }
    printf("You entered: %s\n", cmd); 
}

/*------------------------------------------------------------------------------
* Function: store_stage 
*-----------------------------------------------------------------------------*/
void store_stage( int loc, char *t, char *stages[] ){
    stages[loc] = (char *) malloc(strlen(t) + 1);
    if( stages[loc] == NULL ){
        perror("malloc new stage");
        exit( EXIT_FAILURE );
    }
    strcpy( stages[loc], t );
}
/*------------------------------------------------------------------------------
* Function: check_for_empty_stage 
*-----------------------------------------------------------------------------*/
void check_for_empty_stage( char *tok ){
    int i, err = 1;
    for( i = 0 ; i < strlen(tok) ; i++ ){
        if( !isspace((unsigned char)tok[i]) ){
            err--;
            break;
        }
    }
    if( err ){
        fprintf(stderr, "invalid null comand\n");
        exit( EXIT_FAILURE );
    }
}














