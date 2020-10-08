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
#include "top_words.h"

unsigned long total_uniq_words = 0;
node_t **head_pntr = NULL; /* pointer to the beginning of linked list in 
    hash table */
int lowest_count = 0; /* Keep track of the lowest count to compare new counts
    of words to */
node_t *hash_table[HASH_TABLE_SIZE];

/*------------------------------------------------------------------------------
* Function: print_output 
*
* Description: print_output does exactly that. It receives a pointer to the
*   linked list containing the now k words requested by the user.
*
* param: num_words - number of words requested by the user, or 10 by default
* param: top_words_pntr - pointer to a linked list containing the top words
*-----------------------------------------------------------------------------*/
void print_output(int num_words, node_t *top_words_pntr){
    int i = 0;
    node_t * list_pntr = top_words_pntr;
    printf("The top %d words (out of %ld) are:\n", num_words, total_uniq_words);
    if (list_pntr == NULL){
        return; /* Do not try to access words if there aren't any */
    }
    while ( i < num_words){
        printf("%9d %s\n", list_pntr -> count, list_pntr -> word);
        list_pntr = list_pntr -> next;
        if (list_pntr == NULL){
            break;
        } 
        i++;
    }
}


/*------------------------------------------------------------------------------
* Function: main 
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    char * new_word;
    int NUM_WORDS; /* Number of words requested by the user */
    int stdin_flag = 0; /* Indicate if reading from stdin */
    FILE *file;
    node_t *top_words_pntr; /* Pointer to linked list containing top words */

    NUM_WORDS = read_optional_input(argc, argv); /* Get the number of words the 
            user would like to see before reading the file input */

    /* Cycle through all of the file inputs, or through std in until no more */
    while ( (file = open_file(argc, argv)) != NULL ){ 
        if (stdin != NULL)  /* Check if reading from stdin or a file */
            stdin_flag = 1; /* Set flag if reading from stdin */
        while ( (new_word = read_word(file)) != NULL){ 
            /* Get the next word from file(or stdin), and stop when EOF */
            handle_hash(new_word); /* For every word, send it to 
                be entered into the hash table */
        }
    
        if ( ! stdin_flag ){ /* If reading from a file, close the file */
            if(fclose(file))
                perror(argv[optind-1]); /* Error if cant close the file */
        }
        else if (stdin_flag){ /* Stop reading if EOF was reached on stdin */
            break;
        }
    } /* Repeat the loop until there are no more files to read from */

    /* Now step through the hash_table to find the top words */
    top_words_pntr = top_k_words(NUM_WORDS); /* Get back pointer to list */
    print_output(NUM_WORDS, top_words_pntr); /* Print list */
    return 0;
}


 
