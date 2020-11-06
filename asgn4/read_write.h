/*******************************************************************************
* Filename: read_write.h
*
* Description: This file deals with the direct systems calls for read and write.
*   It handles errors as appropriate.
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _READ_WRITE_H
#define _READ_WRITE_H

/* Standard Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BLOCK_SIZE 512

/* Global Variables */
extern unsigned char readbuf[BLOCK_SIZE]; 
extern unsigned char writebuf[BLOCK_SIZE];

void read_into_buffer( FILE *fd );
int write_buffer_out( FILE *fd );
void write_file_contents( FILE *wrfd, FILE *rdfd, int blocks );

#endif /*_READ_WRITE_H */
