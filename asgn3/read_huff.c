/*******************************************************************************
* Filename: read_huff.c
*
* Description: This file contains the functions involved in deciphering the
*   huffman compressed file. It will read the header and body separately and
*   keep track of where it is in the file and how many bytes it needs to read
*
* Author: Erin Rylie Clark
*******************************************************************************/
/* Standard Header Files */
#include <inttypes.h>

/* Local Header Files */
#include "read_huff.h"
#include "read_write.h"
#include "hist.h"

/*------------------------------------------------------------------------------
* Function: read_header 
*
* Description: read the header and create the histogram table based on the
*   number of uniq_bytes indicated in the first byte.
*-----------------------------------------------------------------------------*/
void read_header( void ){
    int uniq_bytes;
    int count = 0; /* count up to unique bytes to know when to stop */
    int loc = 0; /* Where to read from in readbuf */
    int c = 0; /* the character index */
    int offset; /* To check if there are enough bytes in readbuf to continue */
    int read_bytes; /* Number of read bytes from read(2) */

    /*Read a portion of the file into the buffer */
    read_bytes = read_buffer();

    /* Get the first byte containing the number of unique bytes */
    uniq_bytes = readbuf[loc];
    
    /* If the file has no unique bytes, return an empty file and exit */
    if( uniq_bytes == 0){ 
        printf("The file is empty, or huffman header indicated no unique bytes\
 in file.\n");
        exit( EXIT_SUCCESS );
    }
    loc++; 
    
    /* Now read five bytes, one to get the char, four to get its count */
    while( count <= uniq_bytes ){
        if( (offset = read_bytes - loc) < BYTE_SET ){
            /* Check if the read buffer contains the full next set of bytes */
            /* If not, seek the file descriptor back that number of bytes and
                read again to get the full 5 bytes together */
            lseek( readfd, -offset, SEEK_CUR );
            read_bytes = read_buffer();
            if( read_bytes < BYTE_SET ){ /* Make sure we read enough to get
                the next entry */
                printf("Error: Not a huffman compressed file.\n");
                printf("Not enough entries for claimed unique bytes.\n");
                exit( EXIT_FAILURE );
            }
            loc = 0; /* Read from start of new buffer */
        }

        c = readbuf[loc]; /* Get the character */
        loc++; /* read four byte integer from following location */
        if( hist_table[c] == 0){ /* The character hasnt reaceived a count yet */
            hist_table[c] = read_4_byte_int( loc );
        }
        else{ /* Duplicating a char count - not a huffman compressed file */
            printf("Error: Not a huffman compressed file.\n");
            printf("Duplicate character in header.\n");
            exit( EXIT_FAILURE );
        }        
        count++; /* Keep track of how many entries (5 bytes) have been read */
        loc += 4; /* Account for the 4 byte integer read */
    }
    /* Set file descriptor back to beginning of body so the next function
        can read its own set of bytes. This is mostly to avoid passing 
        read_bytes back */
    lseek( readfd, loc - read_bytes, SEEK_CUR );
}

/*------------------------------------------------------------------------------
* Function: read_4_byte_int
*
* Description: read the integer one byte at a time and shift it into count
*
* param: loc - the location in readbuf to read each char from
*-----------------------------------------------------------------------------*/
int read_4_byte_int( int loc ){
    uint32_t count = 0x00000000;
 
    count = ( readbuf[loc] | count ) << ONE_BYTE;
    count = ( readbuf[loc+1] | count ) << ONE_BYTE;
    count = ( readbuf[loc+2] | count ) << ONE_BYTE;
    count = readbuf[loc+3] | count;

    return count;
}

/*------------------------------------------------------------------------------
* Function: get_next_bit 
*
* Description: mask the current byte to get the bit that we are currently
*   interested in
*
* param: loc - the location to read the byte from in readbuf
* param: bit_count - the current bit we are interested in
*-----------------------------------------------------------------------------*/
char get_next_bit( int loc, unsigned char bit_count ){
    char bit;
    unsigned char mask = 0x80; /* a one in the MSB */
    
    mask = mask >> bit_count; /* Shift to mask the bit we want */
    /* Single out the bit we are interested in and shift it to LSB */
    bit = ( readbuf[loc] & mask ) >> ( (ONE_BYTE -1) - bit_count );
    
    return bit;
}

/*------------------------------------------------------------------------------
* Function: find_leaves_and_write
*
* Description: this function will get each bit from the input file and traverse
*   the tree until it finds a leaf. It will then write the leaf into the
*   writebuf and continute
*
* param: total_chars - the total number of bytes in the file that need to be
*   written based on the histogram table
* param: uniq_bytes - the number of characters in the header of the file
*-----------------------------------------------------------------------------*/
void find_leaves_and_write( unsigned int total_chars, int uniq_bytes){
    unsigned char bit_count = 0; /* Start at MSB of the byte */
    int loc = 0; /* Read at beginning of readbuf */
    int wrloc = 0; /* Write to beginning of writebuf */
    int read_bytes; /* Number of bytes returned by read(2) */
    char bit; /* Bit we are currenty interested in */
    node_t *node_pntr = head_pntr; /* Start at root of tree */
    
    read_bytes = read_buffer(); /* Read the body into the read buffer */
    
    if( uniq_bytes == 1 ){ /* There will not be any codes to the only char */
        while( total_chars > 0 ){ /* Write out the char up to its count */
            writebuf[wrloc] = node_pntr -> c;
            wrloc++;
            total_chars--; /* Keep track of how many left to write */
        }
    }

    while( loc < read_bytes ){ /* If more than one char */
        while( bit_count < BIT_COUNT_MAX ){ /* Get each bit in the byte */
            bit = get_next_bit( loc, bit_count );
            
            if( !bit ){ /* Bit is 0, step down left side of tree */
                node_pntr = node_pntr -> left;
                
                if( check_for_leaf( node_pntr, wrloc ) ){ /* This is a leaf */
                    wrloc++; /* char was written, go to next spot */
                    node_pntr = head_pntr; /* Start at root of tree again */
                    wrloc = check_buffer( wrloc ); /* check for end of buf */
                    total_chars--; /* Keep track of how many left to write */
                } /* Do nothing if it isn't a leaf... continue */
            }
            else{ /* Bit is 1, step down right side of tree */
                node_pntr = node_pntr -> right;
                
                if( check_for_leaf( node_pntr, wrloc ) ){ /* This is a leaf */
                    wrloc++; /* char was written, go to next spot */
                    node_pntr = head_pntr; /* Start at root of tree again */
                    wrloc = check_buffer( wrloc ); /* check for end of buf */
                    total_chars--; /* Keep track of how many left to write */
                } /* Do nothing if it isn't a leaf... continue */
            }

            bit_count++; /* Get next bit */
            if( total_chars == 0 ) /* Break out if we have no more chars left */
                break;
        }

        if( total_chars == 0 ) /* Break out if we have no more chars left */
            break;

        bit_count = 0; /* Reached end of current byte */
        loc++; /* Start at beginning of new byte */

        if( loc == read_bytes ){ /* at end of bytes */
            if( read_bytes == BUFFER_SIZE ){ /* need to read more bytes */
                read_bytes = read_buffer();
                if( read_bytes == 0 && total_chars > 0 ){
                    /* Nothing left to read, but haven't written all chars yet.
                        * Likely not a huffman compressed file */
                    printf("Error: Not a huffman compressed file.\n");
                    printf("Character count is larger than bitstream.\n");
                    exit( EXIT_FAILURE );
                }
                loc = 0;
            }
            else if( read_bytes < BUFFER_SIZE && total_chars > 0 ){
                /* Nothing left to read, but haven't written all chars yet.
                    * Likely not a huffman compressed file */
                printf("Error: Not a huffman compressed file.\n");
                printf("Character count is larger than bitstream.\n");
                exit( EXIT_FAILURE );
            }
        }
    }
    write_buffer(wrloc); /* Write the whole buffer to output */
}

/*------------------------------------------------------------------------------
* Function: check_buffer
*
* Description: Check the write buffer to see if we still have room left to 
*   write to it. If we are out of room, write the buffer to the output and 
*   start writing at the start of the buffer again
*
* param: loc - the number of bytes that have already been written to the buffer
* return: loc - tell the calling function where it should write to next
*-----------------------------------------------------------------------------*/
int check_buffer( int loc ){
    if( loc < BUFFER_SIZE ){
        ; /* Still have room left in buffer */
    }
    else{ /* No room left in buffer */
        write_buffer( loc ); /* Write the buffer to output */
        loc = 0; /* Start writing at beginning of buffer again */
    }
    return loc;
}
/*------------------------------------------------------------------------------
* Function: check_for_leaf
*
* Description: Check if this node is a leaf or a regular parent node
*
* param: node_pntr - a pointer to the struct that we are looking at
* param: wrloc - the location to write the char if it is a leaf
* return: leaf - indicate whether or not it was actually a leaf to the calling
*   function (1 for yes, 0 for no) 
*-----------------------------------------------------------------------------*/
int check_for_leaf( node_t *node_pntr, int wrloc ){
    int leaf;
    if( node_pntr -> c == -1 ){
        leaf = 0; /* not a leaf */
    }
    else{
        leaf = 1; /* it is a leaf */
        /* Write the character to the output buffer */
        writebuf[wrloc] = node_pntr -> c;
    }

    return leaf;
}
