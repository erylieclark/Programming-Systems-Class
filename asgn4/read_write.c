/*******************************************************************************
* Filename: read_write.c
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "read_write.h"

/* Global Variables */
unsigned char readbuf[BLOCK_SIZE];
unsigned char writebuf[BLOCK_SIZE];

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
/*int read_buffer( void ){
    int num_bytes = read( readfd, readbuf, BUFFER_SIZE );
    if( num_bytes == -1 ){
        perror("Read into buffer\n");
        exit( EXIT_FAILURE ); 
    }
    return num_bytes;
}
*/
/*------------------------------------------------------------------------------
* Function: write_buffer 
*
* Description: This function will write the specified number of bytes to the 
*   output. It will return the number of bytes that write(2) actually writes.
* 
* param: num_bytes - the number of bytes that need to be written
* param: writefd - is a global variable, so it will not be passed, but it will
*   be used in this function
* return: num_bytes - the number of bytes written by the write function
*-----------------------------------------------------------------------------*/
int write_buffer( FILE *fd ){ 
    
    int num_bytes = fwrite( writebuf, sizeof(char), BLOCK_SIZE, fd );
    
    if( num_bytes != BLOCK_SIZE ){
        printf("Bytes written not 512.\n");
    }
    
    return num_bytes;
}



