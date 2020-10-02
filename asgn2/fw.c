/*******************************************************************************
* Filename: fw.c
*
* Description: This program will take in a file from stdin and count how many
*   times each word is repeated and output the most used words. The program
*   will ignore case differences in words and will take an optional input from
*   the user to indicate how many words they would like to see. It will also
*   output any errors to indicate if a file could not be opened.
*
* Author: Erin Rylie Clark
*
* References:
*   1. The "hash" function was taken from http://www.cse.yorku.ca/~oz/hash.html
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

#define DEFAULT_NUM_WORDS 10
#define HASH_TABLE_SIZE 20

/* If Defs */
#define READ_WORD

/*------------------------------------------------------------------------------
* Function: read_word
*
*
* To Fix: There will likely be some issue with reading a null for both EOF and
*   a non alpha character. Will need to separate the two so that main knows
*   when to close the file and check for a new one.
*-----------------------------------------------------------------------------*/
#ifdef READ_WORD
char *read_word( FILE *file ){
    static int file_pos = 0;
    int i, len = 0; /* Length of current line */
    char c, *mbuffer; /* c to store cur char, mbuffer to store pntr to line */
    fseek( file, file_pos, SEEK_SET ); /* Adjust file ind to the correct spot
                                        in the file from last call. If first
                                        call, no adjustment will take place. */
    while( ! isalpha(c = fgetc(file)) ){ /* Ignore any non letters */
        if ( c == EOF){ /* Check if non-letter is EOF and return if yes */
            file_pos = 0; /* Reset for next file */
            return NULL;
        }
        else{    
            len++;  /* Keep track of file indicator */
        }
    }
    
    file_pos += len; /* add len of non letters before resetting for a word */
    len = 1; /* Last check of while loop was a letter */
    
    while( isalpha(c = fgetc(file)) ){
        len++;      /* Increase length of line if not '\n' or EOF */
    }
    /* Len is the length of the word at this point */
    len++; /* Inc len to account for last check of while loop - fseek needs to
                know how many spaces to move back from current place, and 
                malloc needs to account for the '\0' that fgets will add to 
                the end of the word */
    mbuffer = malloc(len); /* Allocate enough memory for the word */
    fseek(file, -len, SEEK_CUR); /* Return file indicator to start of word */
    mbuffer = fgets(mbuffer, len, file); /* Copy word into mbuffer */
    file_pos += len; /* Store file indicator pos to set on next call */
    
    for( i=0 ; i<len ; i++ ){ /* Format the word properly */
        if( isupper(mbuffer[i]) ){ /* Convert uppercase letters to lower case */
            mbuffer[i] = (char) tolower(mbuffer[i]); /* place back in buffer */
        }
    }
    return mbuffer;
}
#endif


/*------------------------------------------------------------------------------
* Function: hash
*-----------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
* Function: insert_word 
*-----------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
* Function: read_optional_input 
*
* Known Issues: This function will take in a file that is named with all digits
*   as an argument to -n. The function is not yet checking whether or not the
*   argument is a file or simply a string, but will filter out any arguments
*   that contain non-digit values.
*-----------------------------------------------------------------------------*/
int read_optional_input(int argc, char **argv){
    
    int opt,i;    /* Holds the return value from getopt() */
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
* Function: top_x_words 
*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
* Function: print_output 
*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
* Function: open_file 
*-----------------------------------------------------------------------------*/
FILE *open_file(int argc,char **argv){
    FILE *file;
   
    if ( (argc-optind) < 1 ){       /* Check for more arguments */
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
                file = NULL; /* Last file not opened, end loop in main*/
                break;
            }
        }
        optind++;
        stdin = NULL;
    }
    return file;
}

/*------------------------------------------------------------------------------
* Function: main 
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    char * new_word;
    int NUM_WORDS, stdin_flag = 0;
    FILE *file;
    NUM_WORDS = read_optional_input(argc, argv);
    
    while ( (file = open_file(argc, argv)) != NULL ){
        
        if (stdin != NULL)
            stdin_flag = 1;
        while ( (new_word = read_word(file)) != NULL){
            printf("%s\n",new_word);
            free(new_word);
        }
        if ( ! stdin_flag ){
            if(fclose(file))
                perror(argv[optind-1]);
        }
        else if (stdin_flag){
            break;
        }
    }
    return 0;
}


 
