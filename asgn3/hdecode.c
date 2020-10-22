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
* Function: main
*
* Description: main is written such that its main job is to call the other
*   functions in the order that they are needed. It helps with reading input,
*   and determines if the file is empty, but other than that, does very
*   litte work. 
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    int i, total_chars;
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
        total_chars += hist_table[i];
    }

    create_tree(); /* Readjust list into tree */
    /* print_output(); *//* Helpful for debugging */

    /* Recreate the original file */
    find_leaves_and_write( total_chars, uniq_bytes ); 

    return 0; /* Success */
}
