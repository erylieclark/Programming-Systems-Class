/*******************************************************************************
* hencode.c
*
* Description: This program will take in a file or input from stdin and and
*   compress it using the huffman coding technique.
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

/*------------------------------------------------------------------------------
* Function: main
*
* Description: main simply calls the other functions to do their jobs in order.
*
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    int num_bytes;
    int i = 0;
    int uniq_bytes;
    /* Read input and open files to read and write from */
    parse_input_encode( argc, argv );
    /* Read a portion of the file into the buffer and count its chars */
    while( (num_bytes = read_buffer() ) != 0 ){
        while( i < num_bytes ){
            count_chars( readbuf[i] );
            i++;
        }
        if( num_bytes == BUFFER_SIZE ) /* Filled the read buffer */
            ; /* Continue the loop and read another portion of the file */
        else if( num_bytes < BUFFER_SIZE )
            break; /* Reached end of file before filling the read buffer */
    }
    uniq_bytes = create_list(); /* Create the initial huffman linked list and
        pass back the number of uniq bytes in the hist table */
    /* If the file has no unique bytes, return an empty file and exit */
    if( uniq_bytes == 0){ /* REMINDER: RETURN EMPTY FILE */
        printf("The file is empty.");
        exit( EXIT_SUCCESS );
    }

    return 0;
}
