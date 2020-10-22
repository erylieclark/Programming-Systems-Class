
/*******************************************************************************
* Filename: read_write.c
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "read_write.h"
#include "handle_input.h"
unsigned char readbuf[BUFFER_SIZE];
unsigned char writebuf[BUFFER_SIZE];

/*------------------------------------------------------------------------------
* Function: read_buffer 
*
* Description: This function will read the specified number of bytes and 
*   store them into the read buffer. It will return the number of bytes that
*   read(2) actually reads.
*
* param: readfd - is a global variable, so it will not be passed, but it will
*   be used in this function
* return: num_bytes - the number of bytes read by the read function
*-----------------------------------------------------------------------------*/
int read_buffer( void ){
    int num_bytes = read( readfd, readbuf, BUFFER_SIZE );
    if( num_bytes == -1 ){
        perror("Read into buffer\n");
        exit( EXIT_FAILURE ); /* REMINDER: DO SOME EXTRA HANDLING HERE? */
    /* REMINDER: MAY NEED TO HANDLE THE CASE OF ZERO BYTES READ */
    }
    return num_bytes;
}

/*------------------------------------------------------------------------------
* Function: write_buffer 
*
* Description: This function will write the specified number of bytes and 
*   store them into the write buffer. It will return the number of bytes that
*   write(2) actually reads.
* 
* param: num_bytes - the number of bytes that need to be written
* param: writefd - is a global variable, so it will not be passed, but it will
*   be used in this function
* return: num_bytes - the number of bytes written by the write function
*-----------------------------------------------------------------------------*/
int write_buffer( int num_bytes ){
    int i = 0;
/*    while( i < num_bytes){
        printf("0x%02X\n", writebuf[i]);
        i++;
    }
*/
    num_bytes = write( writefd, writebuf, num_bytes );
    if( num_bytes == -1 ){
        perror("Write into buffer\n");
        exit( EXIT_FAILURE ); /* REMINDER: DO SOME EXTRA HANDLING HERE? */
    }
    return num_bytes;
}



