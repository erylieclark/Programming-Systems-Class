/*******************************************************************************
* File: user_input.c
*
* Description: this contains open_file and read_optional_input to take any
*  input from the user and return a pointer to the location to start reading
*  words from.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#include "user_input.h"
/* Global Variables */
int verb_list = 0;
int strict = 0;

/*------------------------------------------------------------------------------
* Function: validate_options 
*
* Description: This function will take the options with the set bits and check
*   that the 'f' option was given, that only one of 'c', 't', 'x' arguments is
*   give, and that the 'v' argument is only given with the 't' argument.
*
* param: options - the character that holds the set bits from the option inputs 
*-----------------------------------------------------------------------------*/
void validate_options( char options ){
    int CTX = 0; /* Flag to indicate if one of the options has been set */

    /* Check first that f is there */
    if( !(options & FARCH) ){
        printf("Must use the 'f' option.\n");
        printf("usage: [ctxvS]f tarfile [ path [ ... ] ]\n");
        exit( EXIT_FAILURE );
    }
    /* Now check that only one of 'c' 't' 'x' is set */
    if( options & CREATE ){ /* Check for 'c' */
        CTX = 1; /* Set the flag if 'c' was selected */
    }
    if( options & LIST ){ /* Check for 't' */
        if( CTX == 1){ /* Error if c was already set */
            printf("You may only choose one of the 'ctx' options.\n");
            printf("usage: [ctxvS]f tarfile [ path [ ... ] ]\n");
            exit( EXIT_FAILURE );
        }
        else{
            CTX = 1; /* Set flag if 't' was selected and c was not already set*/
        }
    }
    if( options & EXTRACT ){ /* Check for 'x' */
        if( CTX == 1){ /* Error if c or t was already set */
            printf("You may only choose one of the 'ctx' options.\n");
            printf("usage: [ctxvS]f tarfile [ path [ ... ] ]\n");
            exit( EXIT_FAILURE );
        }
        else{
            CTX = 1; /* Set the flag if 'x' was set and ct were not */
        }
    }

    if( CTX == 0 ){ /* If the flag was not set, invalid input */
        printf("You must choose one of the 'ctx' options.\n");
        printf("usage: [ctxvS]f tarfile [ path [ ... ] ]\n");
        exit( EXIT_FAILURE );
    }
    /* Set the global variables for verbose and strict */
    if( options & VERB ){
        verb_list = 1;
    }
    if( options & STRICT ){
        strict = 1;
    }
}

/*------------------------------------------------------------------------------
* Function: read_optional_input 
*
* Description: This function will take the command line input and parse it out
*   to determine if the user has input valid arguments. It will return an error
*   and exit if the input is not valid. 
*
* param: argc, argv - both inputs from the command line
*-----------------------------------------------------------------------------*/
int read_optional_input(int argc, char **argv){
    int i; 
    char options = 0x00; /* Initialize the arguments as none */

    /* First check that there are enough arguments */
    if( argc < MIN_INPUTS ){
        printf("Not enough arguments.\n");
        printf("usage: [ctxvS]f tarfile [ path [ ... ] ]\n");
        exit( EXIT_FAILURE );
    }
    
    /* Now check first argument for validity and set appropriate bits */
    for( i = 0 ; i < strlen( argv[OPT_ARG] ) ; i++ ){
        switch( argv[OPT_ARG][i] ){
            case 'c':
                options = options | CREATE;
                break;
            case 't':
                options = options | LIST;
                break;
            case 'x':
                options = options | EXTRACT;
                break;
            case 'v':
                options = options | VERB;
                break;
            case 'f':
                options = options | FARCH;
                break;
            case 'S':
                options = options | STRICT;
                break;
            default:
                printf("%c is not an option.\n", argv[OPT_ARG][i]);
                printf("usage: [ctxvS]f tarfile [ path [ ... ] ]\n");
                exit( EXIT_FAILURE );
        }
    }

    /* Check that appropriate options were set */    
    validate_options( options );
        
    return options;
}      


/*------------------------------------------------------------------------------
* Function: open_file
*
* Description: This function opens the file and returns a file descriptor if
*   successful, and produces an error if not.
*
* param: argv - the file to attempt to open
* param: mode - the way to open the file - mainly read, write, or read/write.
* 
* return: returns a fd on successful read, exits on failure
*-----------------------------------------------------------------------------*/
FILE *open_file( char *argv, int mode ){
    FILE *fd;
    int check_size = 0; 
    struct stat statbuf = {0};
    switch(mode){
        case LIST: 
            check_size = 1;
            fd = fopen( argv, "r+");
                /* Open as read only, the + means fopen will fail if its a
                    directory */
            break;
        case EXTRACT: 
            check_size = 1;
            fd = fopen( argv, "r+");
                /* Open as read only, the + means fopen will fail if its a
                    directory */
            break;
        case CREATE:
            fd = fopen( argv, "w" );
                /* Open as write only, if file is not there, create a new one.
                    If it is there, truncate it. */
            break;
    }
    if( fd == NULL ){ /* Exit if opening failed */
        perror(argv);
        exit( EXIT_FAILURE );
    }

    if( check_size == 1 ){ /* If reading a tar file, check that the size is a
            multiple of the 512 block size */
        if( (lstat( argv, &statbuf )) == -1 ){ /* Stat the file */
            perror("lstat");
            exit( EXIT_FAILURE );
        }
        if( (statbuf.st_size) < 2*BLOCK_SIZE ){ /* Too small to be tar */
            printf("Not a valid tar file - not enough bytes.\n");
            exit( EXIT_FAILURE );
        }
        else if( (statbuf.st_size % BLOCK_SIZE) ){ /* Check its size */
            printf("Not a valid tar file - size not a multiple of 512.\n");
            exit( EXIT_FAILURE );
        }
        
    }
    return fd;
}


/*------------------------------------------------------------------------------
* Function: get_paths
*
* Description: This function simply holds pointers to all of the paths given
*   by the user in argv. It is used to separate the paths from the other inputs
*   and to have an easy way to pass the paths between functions. If there are
*   no path arguments, it will return a NULL pointer to tell main to simply
*   return an empty file. 
*
* param: paths
* 
* return: returns a pointer to the buffer or NULL if no path arguments
*-----------------------------------------------------------------------------*/
char ** get_paths( int argc, char **argv, int mode ){
    int i = 0; /* Keep track of how many paths we've stored */
    int num_paths = argc - PATH_ARG;
    int act_paths = 0; /* Number of valid paths given from user */
    int cur_path = PATH_ARG;
        /* cur_path is the spot of the path in the arguments list, start at
            first spot where a path could be */
    char **paths;
    struct stat statbuf = {0};
    if( mode == CREATE ){
        if( num_paths == 0 ){
            printf( "No paths given.\n" );
            return NULL;
        }
    }

    /* Create a buffer to hold the pointers to each of the paths */
    if( (paths = (char**) malloc(sizeof(char*)*(num_paths+1))) == NULL ){
            /* Add one to insert a NULL pointer at the end */
        perror( "malloc path buffer" );
        exit( EXIT_FAILURE );
    }
    /* Store pointers into the buffer */
    while( i < num_paths ){
        if( mode == CREATE ){
            /* First check that the path exists */
            if( (lstat( argv[cur_path], &statbuf )) == -1 ){ 
                perror(argv[cur_path]); /* Check return values */
                cur_path++; /* Get the next input argument */
                i++;
                continue; 
            }
        }
        paths[act_paths] = argv[cur_path];
        cur_path++;
        i++;
        act_paths++; /* This path is valid, so add another */
    }

    /* Add a NULL into the buffer after the last path is added */
        /* Could resize the buffer here instead if we wanted */
    
    /* REMINDER: THIS COULD CAUSE A SEGFAULT IF DON'T ADD 1 TO MALLOC */
    paths[act_paths] = NULL;

return paths;

}







