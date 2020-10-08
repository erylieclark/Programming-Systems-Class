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

/*------------------------------------------------------------------------------
* Function: read_optional_input 
*
* Description: This function will take the command line input and parse it out
*   to determine if the user has input valid arguments. It will return an error
*   and exit if the input is not valid. The only optional input this is
*   looking for is a -n flag and a integer value following it. It will not
*   accept nothing, and it will not accept letters.
*
* param: argc, argv - both inputs from the command line
*
* Known Issues: This function will take in a file that is named with all digits
*   as an argument to -n. The function is not yet checking whether or not the
*   argument is a file or simply a string, but will filter out any arguments
*   that contain non-digit values.
*-----------------------------------------------------------------------------*/
int read_optional_input(int argc, char **argv){
    int i; 
    int opt;     /* Holds the return value from getopt() */
    int num_words = DEFAULT_NUM_WORDS;  /* Stores the # given by the user,
                                            init to default value for safety */
    while( (opt = getopt(argc, argv, ":n:")) != -1 ){ /* Get arguments until
                                                            no options left */
        switch(opt){

            case '?':   /* User entered and invalid option */
                printf("No option '%c'.\n",(char) optopt);
                exit(EXIT_FAILURE);

            case 'n':   /* User used the option for -n with an argument */
                for (i = 0; i < strlen(optarg); i++){
                    if ( isdigit(*(optarg + i)) ) /* Only allow digits */
                        /* do nothing */;
                    else{       /* Invalid argument if not all digits */
                        printf("Invalid argument for option -n\n");
                        printf("usage: fw [-n num] [file1 [file2 ...]]\n");
                        exit(EXIT_FAILURE);
                    }
                }
                num_words = atoi(optarg); /* Convert digits to an int */
                break;

            case ':':   /* User did not enter a proper argument */
                printf("Argument required for option -n\n");
                printf("usage: fw [-n num] [file1 [file2 ...]]\n");
                exit(EXIT_FAILURE);
        }
    }
    return num_words;
}      


/*------------------------------------------------------------------------------
* Function: open_file 
*
* Description: open_file takes in the command line arguments and attempts to 
*   open them. If they cannot be opened, it simply skips the file, produces
*   an error, and procedes to try to open the next file if there is one. If 
*   no files were given, it will check if there is any input from stdin and
*   pass that back to main.
*
* param: argc, argv - inputs from the command line
*
*-----------------------------------------------------------------------------*/
FILE *open_file(int argc,char **argv){
    FILE *file; 
    /* Read the remainder of arguments, if there are any */
    if ( (argc-optind) < 1 ){
        if ( stdin != NULL){        /* If no files, check if stdin has val  */
            file = stdin;           /* Read from stdin if yes */
        }
        else if ( argv[optind] == NULL){ /* No more arguments left in argv
                                            and nothing in stdin */
            file = NULL;    /* Pass null to end the loop in main */
        }
    }
    else{   /* If there are more arguments to read from, try to open them */
        while( !(file = fopen(argv[optind],"r")) ){ /* Check for successful
                                                        opening */
            perror(argv[optind]); /* Find the next file that can be opened */
            optind++; /* Inc to check if there is a next argument */
            if ( (argc-optind) < 1){
                file = NULL; /* Last file in list could not be opene*/
                break;
            }
        }
        optind++; /* Next argument in the list */
        stdin = NULL; /* Let main know that it is not reading from stdin */
    }
    return file;
}


