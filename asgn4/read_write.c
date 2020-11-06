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
* Description:  
*
* param: fd - the file stream pointing to the tar file given by the user 
* return: num_bytes - the number of bytes read by the read function
*-----------------------------------------------------------------------------*/
void read_into_buffer( FILE *fd ){
    /* Read the next block */
    int num_bytes = fread( readbuf, sizeof(char), BLOCK_SIZE, fd );

    if( num_bytes == 0 ){ /* Check for error or end of file */
        if( ferror(fd) ){ /* Error */
            printf("Error reading tar file.\n");
            exit( EXIT_FAILURE );
        }
    }
    else if( num_bytes != BLOCK_SIZE ){ /* Must read in blocks of 512 */
        printf("Invalid tar file - not written in block of 512 bytes.\n");
        exit( EXIT_FAILURE );
    }
}

/*------------------------------------------------------------------------------
* Function: write_buffer 
*
* Description: This function will write the specified number of bytes to the 
* 
* param: fd - the file stream pointing to the tar file given by the user 
* return: num_bytes - the number of bytes written by the write function
*-----------------------------------------------------------------------------*/
int write_buffer_out( FILE *fd ){ 
    
    int num_bytes = fwrite( writebuf, sizeof(char), BLOCK_SIZE, fd );
    
    if( num_bytes != BLOCK_SIZE ){
        printf("Bytes written not 512.\n");
    }
    
    return num_bytes;
}

/*------------------------------------------------------------------------------
* Function: write_file_contents 
*
* Description: 
* 
* param: fd - the file stream pointing to the tar file given by the user 
*-----------------------------------------------------------------------------*/
void write_file_contents( FILE *wrfd, FILE *rdfd, int blocks ){
    int i = 0;
    int num_bytes;

    while( i < blocks ){
        /* Clear write buffer */
        memset( writebuf, '\0', BLOCK_SIZE );

        /* Read directly into the writebuf */
        num_bytes = fread( writebuf, sizeof(char), BLOCK_SIZE, rdfd );
        if( num_bytes == 0 ){ /* Check for error */
            if( ferror(rdfd) ){ /* Error */
                printf("Error reading contents of file.\n");
                exit( EXIT_FAILURE );
            }
        }

        /* Write the buffer to the tar file */
        write_buffer_out( wrfd );
        i++;
    }
}    






 
