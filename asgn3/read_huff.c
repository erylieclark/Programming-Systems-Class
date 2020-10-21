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
void read_header( void ){
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
            read_bytes = read_buffer();
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
    /* Set file descriptor back to beginning of body so the next function
        can read its own set of bytes. This is mostly to avoid passing 
        read_bytes back */
    lseek( readfd, loc - read_bytes, SEEK_CUR );
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
    count = ( readbuf[loc] | count ) << ONE_BYTE;
    count = ( readbuf[loc+1] | count ) << ONE_BYTE;
    count = ( readbuf[loc+2] | count ) << ONE_BYTE;
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
char get_next_bit( int loc, unsigned char bit_count ){
    char bit;
    unsigned char mask = 0x80; /* a one in the MSB */
    mask = mask >> bit_count;
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
* param: uniq_bytes - the number of characters in the header of the file
*-----------------------------------------------------------------------------*/
void find_leaves_and_write( int total_chars ){
    unsigned char bit_count = 0; /* Start at MSB of the byte */
    int loc = 0; /* Read at beginning of readbuf */
    int wrloc = 0; /* Write to beginning of writebuf */
    int read_bytes;
    char bit;
    node_t *node_pntr = head_pntr;
    read_bytes = read_buffer(); /* Read the body into the read buffer */
    while( loc < read_bytes ){
        while( bit_count < BIT_COUNT_MAX ){
            bit = get_next_bit( loc, bit_count );
            if( !bit ){ /* Bit is 0, step down left side of tree */
                node_pntr = node_pntr -> left;
                if( check_for_leaf( node_pntr, wrloc ) ){ /* This is a leaf */
                    wrloc++;
                    node_pntr = head_pntr; /* Start at root of tree again */
                    wrloc = check_buffer( wrloc ); /* check for end of buf */
                    total_chars--;
                }
            }
            else{ /* Bit is 1, step down right side of tree */
                node_pntr = node_pntr -> right;
                if( check_for_leaf( node_pntr, wrloc ) ){ /* This is a leaf */
                    wrloc++;
                    node_pntr = head_pntr; /* Start at root of tree again */
                    wrloc = check_buffer( wrloc ); /* check for end of buf */
                    total_chars--;
                }
            }
            bit_count++; /* Get next bit */
            if( total_chars == 0 )
                break;
        }
        if( total_chars == 0 )
            break;
        bit_count = 0; /* Reached end of current byte */
        loc++; /* Start at beginning of new byte */
        if( loc == read_bytes ){ /* at end of bytes */
            if( read_bytes == BUFFER_SIZE ){ /* need to read more bytes */
                read_bytes = read_buffer();
                loc = 0;
            }
            else{
                break; /* No more to read */
            }
        }
    }
    write_buffer(wrloc);
}
/*------------------------------------------------------------------------------
* Function: check_buffer_
*
* Description:
*
* param: uniq_bytes - the number of characters in the header of the file
*-----------------------------------------------------------------------------*/
int check_buffer( int loc ){
    if( loc < BUFFER_SIZE ){
        ; /* Still have room left in buffer */
    }
    else{
        write_buffer( loc );
        loc = 0;
    }
    return loc;
}
/*------------------------------------------------------------------------------
* Function: check_for_leaf
*
* Description:
*
* param: uniq_bytes - the number of characters in the header of the file
*-----------------------------------------------------------------------------*/
int check_for_leaf( node_t *node_pntr, int wrloc ){
    int leaf;
    if( node_pntr -> c == -1 ){
        leaf = 0; /* not a leaf */
    }
    else{
        leaf = 1; /* it is a leaf */
        writebuf[wrloc] = node_pntr -> c;
    }
    return leaf;
}







