/*******************************************************************************
* Filename: read_huff.c
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/
#include <inttypes.h>
#include "read_huff.h"
#include "read_write.h"
#include "hist.h"
/*------------------------------------------------------------------------------
* Function: read_header 
*
* Description: read the header and create the histogram table
*
* param: 
*-----------------------------------------------------------------------------*/
int read_header( void ){
    unsigned char uniq_bytes;
    int count = 0; /* count up to unique bytes to know when to stop */
    int loc = 0;
    int c = 0; /* the character index */
    int offset;
    int read_bytes;
    /*Read a portion of the file into the buffer */
    read_bytes = read_buffer();
    /* Get the first byte containing the number of unique bytes */
    uniq_bytes = readbuf[loc];
    loc++; 
    /* now read five bytes, one to get the char, then its count */
    while( count <= uniq_bytes ){
        if( (offset = read_bytes - loc) < BYTE_SET ){
            /* Check if the read buffer contains the full next set of bytes */
            /* If not, seek the file descriptor back that number of bytes and
                read again to get the full 5 bytes together */
            lseek( readfd, -offset, SEEK_CUR );
            read_buffer();
            loc = 0; /* Read from start of new buffer */
        }
        c = readbuf[loc]; /* Get the character */
        loc++; /* read four byte integer from following location */
        hist_table[c] = read_4_byte_int( loc );
        count++; /* Keep track of how many entries have been read */
        loc += 4; /* Account for the 4 byte integer read */
        /* REMINDER: THERE IS AN ERROR TO ACCOUNT FOR HERE WHERE COUNT WILL
            NOT HAVE REACHED UNIQUE BYTES BUT THERE IS NOTHING LEFT TO READ */
    }
    return loc;
}

/*------------------------------------------------------------------------------
* Function: create_hist 
*
* Description: create the histogram table
*
* param: uniq_bytes - the number of characters in the header of the file
*-----------------------------------------------------------------------------*/
int read_4_byte_int( int loc ){
    uint32_t count = 0x00000000;
    count = ( readbuf[loc] | count ) << 8;
    count = ( readbuf[loc+1] | count ) << 8;
    count = ( readbuf[loc+2] | count ) << 8;
    count = readbuf[loc+3] | count;
    return count;
}


/*------------------------------------------------------------------------------
* Function: get_next_bit 
*
* Description: create the histogram table
*
* param: uniq_bytes - the number of characters in the header of the file
*-----------------------------------------------------------------------------*/
char get_next_bit( int loc ){
    




}






