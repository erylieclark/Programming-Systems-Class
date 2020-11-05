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

int read_buffer( void );
int write_buffer( FILE *fd );

#endif /*_READ_WRITE_H */
