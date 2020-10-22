/*******************************************************************************
* Filename: handle_input.c
*
* Description: This file is mainly for checking that the user entered valid
*   arguments and attempts to opens the files. This file will be the
*   source of any errors related not being able to open the files. If the files
*   are valid, a global file descriptor will be set to the appropriate place.
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "handle_input.h"

int readfd; /* Gloabl variable for read file descriptor */
int writefd; /* Gloabl variable for write file descriptor */

/*------------------------------------------------------------------------------
* Function: parse_input_encode
*
* Description: encode requires at least one input: Read the input from the user.
*   If none specified, error and exit. If  there is at least one, attempt to
*   open the input file. If no output file is specified, read from stdout. If
*   the output file specified is not there, create it. If it is there truncate
*   it. The open_file function will take care of the necessary error handling
*   related to opening the file.
*
* param: argc - # of inputs from the user
* param: argv - array of inputs from the user
* return: no return - the read and write global file descriptors are set
*-----------------------------------------------------------------------------*/
void parse_input_encode( int argc, char **argv ){
    /* First check if there are too many or too few inputs */ 
    if ( argc > MAX_INPUTS ){ 
        printf("Too many inputs.\n");
        printf("usage: hencode infile [ outfile ]\n");
        exit( EXIT_FAILURE ); 
    }
    else if( argc < MIN_INPUTS ){
        printf("Too few inputs.\n");
        printf("usage: hencode infile [ outfile ]\n");
        exit( EXIT_FAILURE ); 
    }

    /* Try to open first file */
    readfd = open_file( argv[INPUT_F], READ_F );
    /* Check for second file */
    if( argc == MAX_INPUTS ){
        writefd = open_file( argv[OUTPUT_F], WRITE_F );
    }
    else{ /* Write to stdout if no output file */
        writefd = STDOUT_FILENO;
    }
}   

/*------------------------------------------------------------------------------
* Function: parse_input_decode
*
* Description: decode does not require any inputs: Read the input from the user,
*   if none specified, read/write from stdin/stdout. If "-" is specified, read
*   from stdin. Otherwise, attempt to open the files. If the output file is not
*   there, create it. If it is there, truncate it. The open_file function will
*   take care of the necessary error handling related to to opening the file.
*
* param: argc - # of inputs from the user
* param: argv - array of inputs from the user
* return: no return - the read and write global file descriptors are set
*-----------------------------------------------------------------------------*/
void parse_input_decode( int argc, char **argv ){
    /* First check if there are too many inputs */ 
    if ( argc > MAX_INPUTS ){ 
        printf("Too many inputs.\n");
        printf("usage: hdecode [ (infile | -) [ outfile ] ]\n");
        exit( EXIT_FAILURE ); 
    }

    /* Check inputs */
    switch( argc ){
        case NO_INPUTS: /* No inputs, read/write from stdin/stdout */  
            readfd = STDIN_FILENO;
            writefd = STDOUT_FILENO;
            break;
        case MIN_INPUTS: /* Input given, but no output */
            if( *argv[INPUT_F] == '-' ) /* No file specified, read from stdin */
                readfd = STDIN_FILENO;
            else /* Otherwise try to open the file */
                readfd = open_file( argv[INPUT_F], READ_F);
            writefd = STDOUT_FILENO; /* Min args given, write to stdout */
            break;
        case MAX_INPUTS: /* Input and output given */
            if( *argv[INPUT_F] == '-' ) /* No file specified, read from stdin */
                readfd = STDIN_FILENO;
            else /* Otherwise try to open the file */
                readfd = open_file( argv[INPUT_F], READ_F);
            writefd = open_file( argv[OUTPUT_F], WRITE_F );
                /* Attempt to open the output file */
            break;
    }
    return;
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
int open_file( char *argv, int mode ){
    int fd;
    
    switch(mode){
        case READ_F:
            fd = open( argv, O_RDONLY);
                /* Open as read only */
            break;
        case WRITE_F:
            fd = open( argv, ( O_WRONLY | O_CREAT | O_TRUNC ), \
                ( S_IRUSR | S_IWUSR ) );
                /* Open as write only, if file is not there, create a new one.
                    If it is there, truncate it. */
            break;
        case RDWRT_F:
            fd = open( argv, ( O_WRONLY | O_CREAT | O_TRUNC ), \
                ( S_IRUSR | S_IWUSR ) );
                /* Open as read/write */
            break;
    }
    if( fd == -1 ){ /* Exit if opening failed */
        perror(argv);
        exit( EXIT_FAILURE );
    }

    return fd;
}


