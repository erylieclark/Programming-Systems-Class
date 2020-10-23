/*******************************************************************************
* hdecode.c
*
* Description: hdecode is used as a file decompressing function. It will take
*   in a huffman compressed file, and turn it back into the original file.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

/* Library Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Local Header Files */
#include "handle_input.h"
#include "read_write.h"
#include "hist.h"
#include "huff_tree.h"
#include "read_huff.h"

/* Constants used in this file */
#define INT_MAX (2^(sizeof(int)*8)) /* Values represented by unsigned int */

/*------------------------------------------------------------------------------
* Function: print_output
*
* Description: This is mainly used as a debugging device and is commented out in
*   main. This function will take an array of pointers to strings and print out
*   the hex value of each character in the file given by the user along with its
*   corresponding huffman code.
*-----------------------------------------------------------------------------*/
void print_output( void ){
    int i;
    char * code;
    for( i = 0 ; i < (HIST_TABLE_SIZE) ; i++ ){
        if (code_table[i] == NULL){
            continue; /* Skip if the entry is empty */
        }
        else{
            code = code_table[i]; /* print the corresponding string */
            printf("0x%02X: %s\n", i , code);
        } 
    }
}
/*------------------------------------------------------------------------------
* Function: check_overflow
*
* Description: Check overflow is used to make sure that the unsigned integer
*   that carries the total number of characters in the file does not exceed 
*   the value that an unsigned int can carry. This function will produce an
*   error if this happens and quit. The two reasons that overflow occurs are:
*       1. The file is too large for the program to handle - this program is
*           not currently made to handle files larger than an unsigned int can
*           carry.
*       2. The user tried to give the program an uncompressed file and the 
*           values accidentally overflowed the unsigned integer.
*-----------------------------------------------------------------------------*/
void check_overflow( unsigned int total_chars, int char_count ){
    if( (INT_MAX - char_count) < total_chars ){
        printf("Error: File too big OR Uncompressed file given.\n");
        exit( EXIT_FAILURE );
    }
}

/*------------------------------------------------------------------------------
* Function: main
*
* Description: main is written such that its main job is to call the other
*   functions in the order that they are needed. It helps with reading input,
*   and determines if the file is empty, but other than that, does very
*   litte work. 
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    int i;
    unsigned int total_chars;
    int uniq_bytes;

    /* Read input and open files to read and write from */
    parse_input_decode( argc, argv );

    /* Read the header and create the histogram table */
    read_header();

    /* Create the initial huffman linked list and pass back the number of uniq
        bytes in the hist table */
    uniq_bytes = create_list();
 
    /* If the file has no unique bytes, return an empty file and exit */
    if( uniq_bytes == 0){ 
        printf("The file is empty.\n");
        exit( EXIT_SUCCESS );
    }

    /* Add up the total number of characters for keeping track of how many
        left in the bit stream */
    for( i = 0 ; i < HIST_TABLE_SIZE ; i++ ){
        check_overflow( total_chars, hist_table[i]); /* Will exit on overflow */
        total_chars += hist_table[i];
    }

    create_tree(); /* Readjust list into tree */
    /* print_output(); *//* Helpful for debugging */

    /* Recreate the original file */
    find_leaves_and_write( total_chars, uniq_bytes ); 

    return 0; /* Success */
}
