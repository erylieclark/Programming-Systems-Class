/*******************************************************************************
* Filename: read_write.h
*
* Description:
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _READ_WRITE_H
#define _READ_WRITE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
extern unsigned char readbuf[BUFFER_SIZE]; 
extern unsigned char writebuf[BUFFER_SIZE];

int read_buffer( void );
int write_buffer( int num_bytes );

#endif /*_READ_WRITE_H */
