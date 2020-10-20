/*******************************************************************************
* Filename: write_huff.c
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/
#include <inttypes.h>
#include "write_huff.h"
#include "read_write.h"

/*------------------------------------------------------------------------------
* Function: write_header 
*
* Description: step through the histogram table and write out the characters
*   and their corresponding counts to the header of the compressed file.
*
* param: uniq_bytes - the number of characters that have a count of at least 1
*-----------------------------------------------------------------------------*/
void write_header_into_buffer( int uniq_bytes ){
    int loc = 0; /* Location to write in buffer */
    int write_bytes = 0; /* Number of bytes to be written to output */
    int i; /* Character to store in the header */
    /* First write the number of unique bytes -1 */
    writebuf[loc] = uniq_bytes - 1;
    /* Now start writing in the bytes and their counts */
    loc++; /* Start writing in next spot */
    for( i = 0 ; i < HIST_TABLE_SIZE ; i++ ){
        if( hist_table[i] == 0 ){
            continue; /* Skip if entry does not have a count */
        }
        else{
            if( !(BUFFER_SIZE > (loc + BYTE_SET)) ){ /* If not enough room left
                in buffer for the next set of char and count (total 5 bytes) */
                write_buffer( write_bytes );
                    /* Write the current buffer to the output file */
                    /* Write_buffer functions handles return values */
                loc = 0; /* Start writing to the start of the buffer again */
                write_bytes = 0; /* Keep track of number of bytes in buffer */
            }
            writebuf[loc] = i; /* Write the character to the buffer */
            loc++; /* Write four byte integer to following location */
            write_4_byte_int( loc, hist_table[i] ); /* Place in big endian */
            loc += sizeof(uint32_t); /* Account for the next four bytes used */
            write_bytes += BYTE_SET; /* Keep track of bytes written */
        }
    }
    write_buffer( write_bytes );
        /* Write the buffer to the output file so that the body can start with
            a new buffer (ie can write to loc 0) */
}

/*------------------------------------------------------------------------------
* Function: write_4_byte_int 
*
* Description: take the count of a character as a 32 bit unsigned integer and
*   write each byte separately into the buffer to maintain network-byte-order.
*
* param: loc - the location to start writing the four bytes
* param: count - the 32 bit unsigned integer representing the count of the char
*-----------------------------------------------------------------------------*/
void write_4_byte_int( int loc,  uint32_t count ){
    writebuf[loc] = ( count >> 24 ) & 0xFF;
    writebuf[loc+1] = ( count >> 16 ) & 0xFF;
    writebuf[loc+2] = ( count >> 8 ) & 0xFF;
    writebuf[loc+3] = count & 0xFF;
}

/*------------------------------------------------------------------------------
* Function: write_body
*
* Description: This function will write out the bitstream of each character in
*   the file. It will first store them into the writebuf buffer and then write
*   the buffer to the output file.
* 
* param: loc - the location to start writing the four bytes
* param: count - the 32 bit unsigned integer representing the count of the char
*-----------------------------------------------------------------------------*/
void write_body( void ){
    int read_bytes;
    int write_bytes = 0;
    int loc = 0;
    int i;
    char *bin_code_pntr;
    unsigned char bit_count = 0;
    /* First seek back to the beginning of the input file to read from the 
        beginning */
    lseek( readfd, 0, SEEK_SET );
    /* Begin reading the characters and enter their bitstream */
    while( (read_bytes = read_buffer() ) != 0 ){ /* Read chunk from file */
        i = 0; /* Start at beginning of buffer */
        while( i < read_bytes ){ /* Still chars in the buffer to read */
            bin_code_pntr = get_bin_code( readbuf[i] ); 
            while( *bin_code_pntr != '\0' ){ /* Go until end of bin code */
                if( *bin_code_pntr == '0' ){
                    wr_bit_0( loc, bit_count );
                    bit_count++; /* Increase after writing the bit */
                }
                else if( *bin_code_pntr == '1' ){
                    wr_bit_1( loc, bit_count );
                    bit_count++; /* Increase after writing the bit */
                }
                else{
                    fprintf(stderr, "Invalid binary value.\n");
                    exit( EXIT_FAILURE );
                }
                if( bit_count == BIT_COUNT_MAX ){ /* At end of current byte */
                    bit_count = 0; /* Reset to start at beginning of new byte */
                    loc++; /* Write to next byte */
                    write_bytes++; /* Keep track of number of bytes to write */
                    if( loc == BUFFER_SIZE ){ /* If not enough room left
                            in buffer */
                        write_buffer( write_bytes );
                            /* Write the current buffer to the output file */
                            /* Write_buffer functions handles return values */
                        loc = 0; /* Write to the start of the buffer again */
                        write_bytes = 0; /* Keep track of number of bytes in
                                            buffer */
                    }
                }
                bin_code_pntr++;
            }
            i++;
        }
        if( read_bytes == BUFFER_SIZE ) /* Filled the read buffer */
            ; /* Continue the loop and read another portion of the input file */
        else if( read_bytes < BUFFER_SIZE )
            break; /* Reached end of file before filling read buffer */
    }
    write_buffer( write_bytes );
        /* Finish by writing the remainder of the buffer to output */
}

/*------------------------------------------------------------------------------
* Function: wr_bit_0 
*
* Description: write a 0 to the correct position in the location provided
* param: loc - the byte to write to
* param: bit_count - the bit to write to... bit 01234567...
*-----------------------------------------------------------------------------*/
void wr_bit_0( int loc, unsigned char bit_count ){
    unsigned char mask = 0x80;
    mask = mask >> bit_count;
    mask = ~mask;
    writebuf[loc] = writebuf[loc] & mask;
}

/*------------------------------------------------------------------------------
* Function: wr_bit_1 
*
* Description: write a 1 to the correct position in the location provided
* param: loc - the byte to write to
* param: bit_count - the bit to write to... bit 01234567...
*-----------------------------------------------------------------------------*/
void wr_bit_1( int loc, unsigned char bit_count ){
    unsigned char mask = 0x80;
    mask = mask >> bit_count;
    writebuf[loc] = writebuf[loc] | mask;
}

/*------------------------------------------------------------------------------
* Function: get_bin_code
*
* Description: this function takes the current character of the input file,
*   finds its binary code in the code table, and returns a pointer to it.
* 
* param: c - current character from input file
* return: bin_code_pntr - a pointer to the binary code string
*-----------------------------------------------------------------------------*/
char * get_bin_code( unsigned char c ){
    char *bin_code_pntr = code_table[c];
    return bin_code_pntr;
}





