/*******************************************************************************
* Filename: read_huff.h
*
* Description: This file handles reading the compressed file and turning it
*   back into the original file.
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _READ_HUFF_H
#define _READ_HUFF_H

/* Standard Header Files */
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
#define ONE_BYTE 8 /* Bits in byte */
#define BIT_COUNT_MAX 8 /* Bits in byte */

void read_header( void );
int read_4_byte_int( int loc );
char get_next_bit( int loc, unsigned char bit_count );
void find_leaves_and_write( int total_chars, unsigned char uniq_bytes );
int check_buffer( int loc );
int check_for_leaf( node_t *node_pntr, int wrloc );

#endif /*_READ_HUFF_H */
