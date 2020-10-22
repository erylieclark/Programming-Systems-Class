/*******************************************************************************
* hdecode.c
*
* Description: This program will take in a file or input from stdin and 
*   uncompress it.
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

/*------------------------------------------------------------------------------
* Function: print_output
*
* Description: This function will take an array of pointers to strings and 
*   print out the hex value of each character in the file given by the user
*   along with its corresponding huffman code.
*
* param: array - the array that holds the pointers to characters
*
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
* Function: main
*
* Description: main simply calls the other functions to do their jobs in order.
*
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    int i, total_chars;
    unsigned char uniq_bytes;
    /* Read input and open files to read and write from */
    parse_input_decode( argc, argv );
    /* Read the header and create the histogram table */
    read_header();
    /* Create the initial huffman linked list and pass back the number of uniq
        bytes in the hist table */
    uniq_bytes = create_list(); 
    /* If the file has no unique bytes, return an empty file and exit */
    if( uniq_bytes == 0){ /* REMINDER: RETURN EMPTY FILE */
        printf("The file is empty.\n");
        exit( EXIT_SUCCESS );
    }
    /* Add up the total number of characters */
    for( i = 0 ; i < HIST_TABLE_SIZE ; i++ ){
        total_chars += hist_table[i];
    }
    create_tree(); /* Readjust list into tree */
    /* print_output(); */
    find_leaves_and_write( total_chars, uniq_bytes );
    return 0;
}
