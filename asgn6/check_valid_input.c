/*******************************************************************************
* Filename: check_valid_input.c
*
* Description: these functions mainly deal with the separating out of the
*   command line into information that can be more easily stored later on. They
*   also do the majority of the usage error checking 
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

/* Local Header Files */
#include "check_valid_input.h"
#include "parseline.h"

#define OTHER

/*------------------------------------------------------------------------------
* Function: check_redirs 
*
* Description: the main purpose of this function is to determine if there are
*   too many redirections, ambiguous redirections if the stage is being piped
*   at all, and to signal an error if there are any issues.
*
* param: stage_tokens - the individual pieces of the stage, stored as strings,
*   including the redirection symbols
* param: num_tokens - the number of strings in the stage_tokens array
* param: pipe_status - indicates if this stage has any pipes, and what they are
*-----------------------------------------------------------------------------*/
int check_redirs( char *stage_tokens[] , int num_tokens, int pipe_status){
    int r_in = 0; /* Count input redirection symbols */
    int r_out = 0; /* Count output redirection symbols */
    int redir_flag = 0; /* Check if the last argument was a redir or cmd */
    int mult_args = 0;
    int i = 0;
    char *token;

    /* Cycle through the tokesn and determine if they are in an appropriate
        order. Generally, a redirection symbol must have arguments on either
        side of it */
    for( i = 0 ; i < num_tokens ; i++ ){
        token = stage_tokens[i];
        if( strcmp( token, "<" ) == 0 ){
            r_in++; /* Increase the count of the redir symbol */
            redir_flag = 1; 
            /* Check that the first tok is not a redirection symbol */
            if( i == 0 ){ /* First tok is a redir */
                fprintf(stderr, "%s: bad input redirection\n", 
                stage_tokens[0]);
                return -1;
            }
            else if( i == (num_tokens - 1) ){ /* Last tok is a redir */
                fprintf(stderr, "%s: bad input redirection\n", 
                    stage_tokens[0]);
                return -1;
            }
        }
        else if( strcmp( token, ">" ) == 0 ){
            r_out++; /* Increase count of the redir symbol */
            redir_flag = 1;
            /* Check that the first tok is not a redirection symbol */
            if( i == 0 ){ /* First tok is a redir */
                fprintf(stderr, "%s: bad output redirection\n", 
                    stage_tokens[0]);
                return -1;
            }
            else if( i == (num_tokens - 1) ){ /* Last tok is a redir */
                fprintf(stderr, "%s: bad output redirection\n",
                    stage_tokens[0]);
                return -1;
            }
        }
        else{ /* If not a symbol, this is some type of argument */
            if( !redir_flag && i != 0 ){ /* If this is not the first token,
                    and the last token was not a redir sybmol, it is a 
                    command argument */
                mult_args++;
                if( (mult_args + 1) >= MAX_CMD_ARGS ){ /* Check if this
                        exceeds the limit */
                    fprintf(stderr, "%s: too many arguments\n",
                        stage_tokens[0]);
                    return -1;
                }
            }
            else{ /* If the last token was a redir symbol, this is the first
                    argument following it, so lower the flag */
                redir_flag = 0;
            }
        }
    }

    /* Now check for too many redirections */
    if( r_in > 1 ){
        fprintf(stderr, "%s: bad input redirection\n", stage_tokens[0] );
        return -1;
    }
    else if( r_out > 1 ){
        fprintf(stderr, "%s: bad output redirection\n", stage_tokens[0] );
        return -1;
    }
    
    /* Check that redirections are apropriate for where the stage is
         in the pipeline */    
    switch( pipe_status ){
        case NO_PIPE:
            /* Do nothing, both redirections can be present in this case */
            break;
        case PIPE_IN: /* if an input redir symbol exists in the stage, but
                something is being piped in, its ambiguous */
            if( r_in ){
                fprintf(stderr, "%s: ambiguous input\n", stage_tokens[0] );
                return -1;
            }
            break;
        case PIPE_OUT: 
            if( r_out ){
                fprintf(stderr, "%s: ambiguous output\n", stage_tokens[0] );
                return -1;
            }
            break;
        case PIPE_IN_OUT:
            if( r_in ){
                fprintf(stderr, "%s: ambiguous input\n", stage_tokens[0] );
                return -1;
            }
            if( r_out ){
                fprintf(stderr, "%s: ambiguous output\n", stage_tokens[0] );
                return -1;
            }
            break;
    }
    return 0; 
}
/*------------------------------------------------------------------------------
* Function: parse_by_spaces
*
* Description: This function uses strtok to separate out each of the stages
*   based on spaces. It then stores each of the strings into an array of
*   pointers to strings.
*
* param: stage - the string to be separated
* param: stage_tokens - where to store the separated strings
* return: an integer indicating the number of tokens found in the stage, or
*   -1 on failure
*-----------------------------------------------------------------------------*/
int parse_by_spaces( char *stage, char *stage_tokens[] ){
    int i = 0;
    char *token;
    int res = 0;
    token = strtok( stage, SPACE_DELIM );
    do{
        if( i == MAX_STAGE_TOKENS ){
            fprintf(stderr, "%s: too many arguments\n", stage_tokens[0]);
            res = -1; 
            break;
        }
        if( store_string( i, token, stage_tokens ) ){ /* -1 on failure */
            res = -1; /* Malloc failed */
            break;
        }
        i++;
    }while( (token = strtok( NULL, SPACE_DELIM )) );
    
    if( res == -1 ){ /* Check for errors */
        while( i ){ /* Free all of the allocated memory before exit */
            free( stage_tokens[--i] );
        }
        return -1;
    }
    
    stage_tokens[i] = NULL;
    return i; /* Number of tokens */
}
/*------------------------------------------------------------------------------
* Function: trim_trailing_space 
*
* Description: this function is used to take off any spaces, newlines, or other
*   whitespace characters from the ends of the stage strings
*
* param: str - the string to modify
* return: the modified string
*-----------------------------------------------------------------------------*/
char * trim_trailing_space( char *str ){
    int last = strlen(str) - 1;
    while( isspace(str[last]) ){
        str[last] = '\0';
        last--;
    }
    return str;
}
/*------------------------------------------------------------------------------
* Function: parse_by_pipe
*
* Description: This function uses strtok to separate out the command line based
*   on the '|' symbol, it then trims off any whitespace off the end of the 
*   strings and stores the strings in an array of pointers to strings.
*
* param: cmd - the command line to parse
* param: stages - an array of pointers to strings that each stage will be
*   stored into
* return: an integer containing the number of pipes found, -1 on failure
*-----------------------------------------------------------------------------*/
int parse_by_pipe( char cmd[], char *stages[] ){
    int res = 0;
    int i = 0;
    char *token;

    token = strtok( cmd, PIPE_DELIM );
    do{
        if( i == MAX_STAGES ){ /* Check if it will overflow the array */
            fprintf(stderr, "pipeline too deep\n");
            res = -1;
            break;
        }
        if( check_for_empty_stage( token ) ){ /* 0 on success, -1 on failure */
            res = -1;
            break;
        }
        token = trim_trailing_space(token);
        /* Malloc and store the string */
        if( store_string( i, token, stages ) ){ /* -1 on failure */
            res = -1; /* Malloc failed */
            break;
        }
        i++;
    }while( (token = strtok( NULL, PIPE_DELIM )) ); /* read from same string */
    
    if( res == -1 ){ /* Check for errors */
        while( i ){ /* Free all of the allocated memory before exit */
            free( stages[--i] );
        }
        return -1;
    }
    
    stages[i] = NULL; /* Terminate the list with NULL */
    return i; /* Number of stages */
}
/*------------------------------------------------------------------------------
* Function: check_first_char 
*
* Description: when parsing by pipe, it won't indicate that the user used a
*   pipe at the beginning, so the first character needs to be checked for it
*   manually
*
* param: c - the char to check
*-----------------------------------------------------------------------------*/
int check_first_char( char c ){

    /* Check that first char isn't a redirection or pipe */
    if( c == '|' ){ 
        fprintf(stderr, "no command given\n");
        return -1;
    }
    else if( c == '<' ){
        fprintf(stderr, "bad input redirection\n");
        return -1;
    
    }
    else if( c == '>' ){
        fprintf(stderr, "bad output redirection\n");
        return -1;
    }
    return 0;
}

/*------------------------------------------------------------------------------
* Function: read_into_buffer
*
* Description: read the original command line from stdin into the cmd buffer
*   for later parsing
*
* param: cmd - the buffer to hold the command in
* return: 0 on success, -1 on failure 
*-----------------------------------------------------------------------------*/
int read_into_buffer( char cmd[] ){
    int c, i = 0;
    sigset_t block_mask;
    sigset_t ublock_mask;
    sigemptyset(&ublock_mask);
    sigemptyset(&block_mask);
    sigaddset( &block_mask, SIGINT );
#ifdef UGH
/* Read from stdin and store into a buffer */
    sigprocmask( SIG_SETMASK, &block_mask, NULL );
    while( (c = getchar()) != EOF ){
        sigprocmask( SIG_SETMASK, &ublock_mask, NULL );
        if( sig_flag ){
            printf("\nDetected Ctrl C: returning...\n");
            sig_flag = 0;
            return -1;
        }
        switch( c ){
            case '\n':
               cmd[i] = '\0';
               return 0;
            default: /* Normal char */
                cmd[i] = c;
                break;
        }
        i++;
        if( i == MAX_CMD_LENGTH ){
            fprintf(stderr, "command too long\n");
            fflush(stdin); /* Get rid of the remaining input if any */
            return -1;
        }
        sigprocmask( SIG_SETMASK, &block_mask, NULL );
    }
    sigprocmask( SIG_SETMASK, &ublock_mask, NULL );
    if( sig_flag ){
        printf("\nDetected Ctrl C: returning...\n");
        sig_flag = 0;
        return -1;
    }
    if( feof(stdin) ){ /* Check for end of file */
        printf("\nPressed ^D: exiting...\n");
        exit( EXIT_SUCCESS );
    }
    else if( ferror(stdin) ){
        perror("getchar");
        exit( EXIT_FAILURE );
    }
#endif
#ifdef OTHER
    if( (fgets( cmd, MAX_CMD_LENGTH, stdin )) == NULL ){
        if( sig_flag ){
            printf("\nDetected Ctrl C: returning...\n");
            sig_flag = 0;
            return -1;
        }
        if( feof(stdin) ){ /* Check for end of file */
            printf("\nPressed ^D: exiting...\n");
            exit( EXIT_SUCCESS );
        }
        else{
            perror("fgets");
            exit( EXIT_FAILURE );
        }
    }
    if( sig_flag ){
        printf("\nDetected Ctrl C: returning...\n");
        sig_flag = 0;
        return -1;
    }

    /* Check that it does not overflow the buffer */
    else if( strchr( cmd, '\n' ) == NULL ){
        fprintf(stderr, "command too long\n");
        fflush(stdin); /* Get rid of the remaining input if any */
            /* Will need to not exit here in mush? */
        return -1;
    }

#endif
    return 0;
}

/*------------------------------------------------------------------------------
* Function: store_string 
*
* Description: malloc a buffer the length of the given string, plus 1 for the
*   null termination, and copy the string into the buffer
*
* param: loc - location in the str buffer to store the string
* param: tok - the token to store into the buffer
* param: str - an array of pointers to strings, where the string will be stored
* return: -1 on failure, 0 on success
*-----------------------------------------------------------------------------*/
int store_string( int loc, char *tok, char *str[] ){
    str[loc] = (char *) malloc(strlen(tok) + 1);
    if( str[loc] == NULL ){ /* Check return values */
        perror("malloc new stage");
        return -1;
    }
    strcpy( str[loc], tok );
    return 0;
}
/*------------------------------------------------------------------------------
* Function: check_for_empty_stage 
*
* Description: check if there are no characters between the pipes. If there
*   are only whitespace characters, this is invalid 
*
* param: tok - the string to check if it is only whitespace chars
* return: 0 on success, -1 on failure
*-----------------------------------------------------------------------------*/
int check_for_empty_stage( char *tok ){
    int i, err = 1;
    for( i = 0 ; i < strlen(tok) ; i++ ){
        if( !isspace((unsigned char)tok[i]) ){
            err--;
            break;
        }
    }
    if( err ){
        fprintf(stderr, "invalid null comand\n");
        return -1;
    }
    return 0;
}

