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
* Function: check_redirs 
*-----------------------------------------------------------------------------*/
void check_redirs( char *stage_tokens[] , int num_tokens, int pipe_status){
    int r_in = 0;
    int r_out = 0;
    int redir_flag = 0; /* Check if the last argument was a redir or cmd */
    int mult_args = 0;
    int i = 0;
    char *token;

    for( i = 0 ; i < num_tokens ; i++ ){
        token = stage_tokens[i];
        if( strcmp( token, "<" ) == 0 ){
            if( ! mult_args ){
                r_in++;
                redir_flag = 1;
                /* Check that the first one is not a redirection symbol */
                if( i == 0 ){ /* First arg is a redir */
                    fprintf(stderr, "%s: bad input redirection\n", 
                        stage_tokens[0]);
                    exit( EXIT_FAILURE );
                }
                else if( i == (num_tokens - 1) ){ /* Last arg is a redir */
                    fprintf(stderr, "%s: bad input redirection\n", 
                        stage_tokens[0]);
                    exit( EXIT_FAILURE );
                }
            }
            else{
                fprintf(stderr, "%s: bad input redirection\n", 
                    stage_tokens[0]);
                exit( EXIT_FAILURE );
            }
        }
        else if( strcmp( token, ">" ) == 0 ){
            if( ! mult_args ){
                r_out++;
                redir_flag = 1;
                /* Check that the first one is not a redirection symbol */
                if( i == 0 ){ /* First arg is a redir */
                    fprintf(stderr, "%s: bad output redirection\n", 
                        stage_tokens[0]);
                    exit( EXIT_FAILURE );
                }
                else if( i == (num_tokens - 1) ){ /* Last arg is a redir */
                    fprintf(stderr, "%s: bad output redirection\n",
                        stage_tokens[0]);
                    exit( EXIT_FAILURE );
                }
            }
            else{
                fprintf(stderr, "%s: bad output redirection\n",
                    stage_tokens[0]);
                exit( EXIT_FAILURE );
            }
        }
        else{
            if( !redir_flag && i != 0 ){
                mult_args++;
                if( (mult_args + 1) >= MAX_CMD_ARGS ){
                    fprintf(stderr, "%s: too many arguments\n",
                        stage_tokens[0]);
                    exit( EXIT_FAILURE );
                }
            }
            else{
                redir_flag = 0;
            }
        }
    }

    /* Now check for too many redirections */
    if( r_in > 1 ){
        fprintf(stderr, "%s: bad input redirection\n", stage_tokens[0] );
        exit( EXIT_FAILURE );
    }
    else if( r_out > 1 ){
        fprintf(stderr, "%s: bad output redirection\n", stage_tokens[0] );
        exit( EXIT_FAILURE );
    }
    
    /* Now check that redirections are apropriate for where the stage is
         in the pipeline */    
    switch( pipe_status ){
        case NO_PIPE:
            /* Do nothing, both redirections can be present in this case */
            break;
        case PIPE_IN:
            if( r_in ){
                fprintf(stderr, "%s: ambiguous input\n", stage_tokens[0] );
                exit( EXIT_FAILURE );
            }
            break;
        case PIPE_OUT:
            if( r_out ){
                fprintf(stderr, "%s: ambiguous output\n", stage_tokens[0] );
                exit( EXIT_FAILURE );
            }
            break;
        case PIPE_IN_OUT:
            if( r_in ){
                fprintf(stderr, "%s: ambiguous input\n", stage_tokens[0] );
                exit( EXIT_FAILURE );
            }
            if( r_out ){
                fprintf(stderr, "%s: ambiguous output\n", stage_tokens[0] );
                exit( EXIT_FAILURE );
            }
            break;
    } 
}
/*------------------------------------------------------------------------------
* Function: parse_by_spaces
*-----------------------------------------------------------------------------*/
int parse_by_spaces( char *stage, char *stage_tokens[] ){
    int i = 0;
    char *token;
    
    token = strtok( stage, SPACE_DELIM );
    do{
        store_string( i, token, stage_tokens );
        i++;
        if( i > MAX_STAGE_TOKENS ){
            fprintf(stderr, "%s: too many arguments\n", stage_tokens[0]);
            exit( EXIT_FAILURE );
        }
    }while( (token = strtok( NULL, SPACE_DELIM )) );
    /*store_string( i, NULL, stage_tokens );*/ /* Store a null in last spot */
    return i; /* Number of tokens */
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
        store_string( i, token, stages );
        i++;
        if( i > MAX_STAGES ){
            fprintf(stderr, "pipeline too deep\n");
            exit( EXIT_FAILURE );
        }
    }while( (token = strtok( NULL, PIPE_DELIM )) );
    /*store_string( i, NULL, stages );*/ /* Store Null in last spot */
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
}

/*------------------------------------------------------------------------------
* Function: store_string 
*-----------------------------------------------------------------------------*/
void store_string( int loc, char *tok, char *str[] ){
    str[loc] = (char *) malloc(strlen(tok) + 1);
    if( str[loc] == NULL ){
        perror("malloc new stage");
        exit( EXIT_FAILURE );
    }
    strcpy( str[loc], tok );
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














