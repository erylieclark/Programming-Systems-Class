/*******************************************************************************
* Filename: write_huff.h
*
* Description: This file handles turning the code_table into the actual binary
*   encoded file.
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _WRITE_HUFF_H
#define _WRITE_HUFF_H

/* Standard Headers Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

/* Local Header Files */
#include "read_write.h"
#include "hist.h"
#include "huff_tree.h"
#include "handle_input.h"


#define BYTE_SET 5 /* Number of bytes in a char and int pair for the header */
#define BIT_COUNT_MAX 8 /* Bits in a byte */

void write_header_into_buffer( int uniq_bytes );
void write_4_byte_int( int loc, uint32_t c );
void write_body( void );
void pad_bits( int loc, unsigned char bit_count );
void wr_bit_0( int loc, unsigned char bit_count );
void wr_bit_1( int loc, unsigned char bit_count );
char *get_bin_code( unsigned char c );

#endif /*_WRITE_HUFF_H */
