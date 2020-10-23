/*******************************************************************************
* hencode.c
*
* Description: hencode is meant to be used as a file compression program. It
*   will take in a file or read from stdin and produce a huffman compressed
*   version of the file to a specified file, or to stdout.
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
#include "write_huff.h"

/*------------------------------------------------------------------------------
* Function: print_output
*
* Description: This is mostly meant as a debugging feature in this program. 
*   This function will take an array of pointers to strings and print out the
*   hex value of each character in the file given by the user along with its
*   corresponding huffman code. code_table is a global array.
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
    int num_bytes; /* number of bytes read from read(2) */
    int loc; /* Location in the buffer to read from */
    int uniq_bytes; /* Unique bytes in the file, not to be bigger than 256 */
    /* Read input and open files to read and write from */
    parse_input_encode( argc, argv );
    /* Read a portion of the file into the buffer and count its chars */
    while( (num_bytes = read_buffer() ) != 0 ){
        loc = 0; /* Start at beginning of buffer */
        while( loc < num_bytes ){ /* Read until nothing left in buffer */
            count_chars( readbuf[loc] );
            loc++;
        }
        if( num_bytes == BUFFER_SIZE ) /* Filled the read buffer */
            ; /* Continue the loop and read another portion of the file */
        else if( num_bytes < BUFFER_SIZE )
            break; /* Reached end of file before filling the read buffer */
    }
    uniq_bytes = create_list(); /* Create the initial huffman linked list and
        pass back the number of uniq bytes in the hist table */
    /* If the file has no unique bytes, return an empty file and exit */
    if( uniq_bytes == 0){
        printf("The file is empty.");
        exit( EXIT_SUCCESS );
    }
    create_tree(); /* Readjust list into tree */
    collect_codes(); /* Go through tree and collect code for each char */
    /*print_output();*/ /* Prints the codes for each char out to view */
    write_header_into_buffer( uniq_bytes );
    write_body(); /* Write the bitstream */

    return 0; /* Success */
}
