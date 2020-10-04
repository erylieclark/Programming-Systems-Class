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
#include "user_input.h"
#include "read_word.h"
#include "hashing.h"



/*------------------------------------------------------------------------------
* Function: top_x_words 
*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
* Function: print_output 
*-----------------------------------------------------------------------------*/
void print_output(int num_words, int total_words, int count, char *key){
    /* printf("The top %d words (out of %d) are:\n", num_words, total_words); */
    printf("%9d %s\n", count, key);
}


/*------------------------------------------------------------------------------
* Function: main 
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    char * new_word;
    int NUM_WORDS, stdin_flag = 0;
    FILE *file;
    /*unsigned long total_words = 0;*/


    NUM_WORDS = read_optional_input(argc, argv); /* Get the number of words the 
            user would like to see before reading the file input */
    
    printf("Number of words requested: %d\n", NUM_WORDS);

    while ( (file = open_file(argc, argv)) != NULL ){ /* Open next file if there
                                                            is one */
        if (stdin != NULL)  /* Check if reading from stdin or a file */
            stdin_flag = 1; /* Set flag if reading from stdin */
        while ( (new_word = read_word(file)) != NULL){ 
            /* Get the next word from file(or stdin), and stop when EOF */
            handle_hash(new_word); 
        }
    
        if ( ! stdin_flag ){ /* If reading from a file, close the file */
            if(fclose(file))
                perror(argv[optind-1]); 
        }
        else if (stdin_flag){ /* Stop reading if EOF was reached on stdin */
            break;
        }
    }
    return 0;
}


 
