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
* Description: this function is intended to read a tarfile and read blocks of
*   512 bytes into the read buffer so that the calling function can use it. It
*   willl check that  
*
* param: fd - the file stream pointing to the tar file given by the user
* error: this function will exit on error because we can't do anything
*   if we can't read the tarfile 
*-----------------------------------------------------------------------------*/
void read_into_buffer( FILE *fd ){
    /* Read the next block */
    int num_bytes = fread( readbuf, sizeof(char), BLOCK_SIZE, fd );

    if( num_bytes == 0 ){ /* Check for error or end of file */
        if( ferror(fd) ){ /* Error */
            printf("Error reading tar file.\n");
            exit( EXIT_FAILURE );
        }
    } /* Don't care about end of file, other functions will handle it */
    else if( num_bytes != BLOCK_SIZE ){ /* Must read in blocks of 512 */
        printf("Invalid tar file - not written in block of 512 bytes.\n");
        exit( EXIT_FAILURE );
    }
}
/*------------------------------------------------------------------------------
* Function: write_buffer_out 
*
* Description: This function will write blocks of data to the tarfile taken from
*   the global write buffer.
* 
* param: fd - the file stream pointing to the tar file given by the user 
* return: num_bytes - the number of bytes written by the write function
* error: This function will exit on error because we can't do anything if we
*   cannot correctly write to the tarfile
*-----------------------------------------------------------------------------*/
int write_buffer_out( FILE *fd ){ 
    
    /* Write to the tar file */
    int num_bytes = fwrite( writebuf, sizeof(char), BLOCK_SIZE, fd );
    
    /* Check for errors */
    if( num_bytes == 0 ){ /* Check for error or end of file */
        if( ferror(fd) ){ /* Error */
            printf("Error reading tar file.\n");
            exit( EXIT_FAILURE );
        }
        if( feof(rdfd) ){ /* End of file */
            printf("Premature end of file indicator.\n");
            exit( EXIT_FAILURE );
        }
    } /* Don't care about end of file, other functions will handle it */
    else if( num_bytes != BLOCK_SIZE ){
        printf("Bytes written not 512.\n");
        exit( EXIT_FAILURE );
    }
    
    return num_bytes;
}

/*------------------------------------------------------------------------------
* Function: write_file_contents 
*
* Description: this function is intended to take the file contents from a file
*   that is being archived, and transfer them into the tarfile following the
*   header for the file 
* 
* param: fd - the file stream pointing to the tar file given by the user 
* error: this function exits on error because we can't continue if the file
*   cannot be transfered correctly, it will cause an invalid tarfile
*-----------------------------------------------------------------------------*/
void write_file_contents( FILE *wrfd, FILE *rdfd, int blocks ){
    int i = 0;
    int num_bytes;

    while( i < blocks ){ /* How many 512 byte blocks need to be written */
        /* Clear write buffer to initialize before reading into it */
        memset( writebuf, '\0', BLOCK_SIZE );

        /* Read directly into the writebuf */
        num_bytes = fread( writebuf, sizeof(char), BLOCK_SIZE, rdfd );
        if( num_bytes == 0 ){ /* Check for error */
            if( ferror(rdfd) ){ /* Error */
                printf("Error reading contents of file.\n");
                exit( EXIT_FAILURE );
            }
            if( feof(rdfd) ){ /* End of file */
                printf("Premature end of file indicator.\n");
                exit( EXIT_FAILURE );
            }
        }

        /* Write the buffer to the file */
        write_buffer_out( wrfd );
        i++;
    }
}    
/*------------------------------------------------------------------------------
* Function: restore_file_contents 
*
* Description: this function takes pointers to the tarfile and the file to be
*   written to, and transfers the correct number of bytes into the new file
*   directly from the tarfile to restore it. 
* 
* param: rdfd - the file stream pointing to the tar file given by the user
* param: wrfd - the file to restore the contents into
* param: blocks - the number of 512 byte blocks that need to be transferred
* param: size - the size of the file, to make sure that the last portion is
*   not transferred as a full block if it does not fill the block
*-----------------------------------------------------------------------------*/
void restore_file_contents( FILE *wrfd, FILE *rdfd, int blocks, int size ){
    int i = 0;
    int num_bytes;
    int leftover; /* We are reading from blocks but do not want to write in
        blocks because this can cause extra bytes to be written back to the
        file. Calc the leftover bytes that do not fill a full block */
    if( (leftover = size % BLOCK_SIZE) ){
        blocks--; /* Remove one from blocks b/c the last one won't be full */
    }
    while( i < blocks ){ /* Read and write the full blocks */
        /* Clear write buffer */
        memset( writebuf, '\0', BLOCK_SIZE );

        /* Read directly into the writebuf */
        num_bytes = fread( writebuf, sizeof(char), BLOCK_SIZE, rdfd );
        if( num_bytes == 0 ){ /* Check for error */
            if( ferror(rdfd) ){ /* Error */
                printf("Error reading contents of file.\n");
                exit( EXIT_FAILURE );
            }
            if( feof(rdfd) ){ /* End of file */
                printf("Premature end of file indicator.\n");
                exit( EXIT_FAILURE );
            }
        }

        /* Write the buffer to the file */
        write_buffer_out( wrfd );
        i++;
    }
    if( leftover != 0 ){ /* If there are leftover bytes */
        /* Clear write buffer */
        memset( writebuf, '\0', BLOCK_SIZE );

        /* Read directly into the writebuf only the leftover bytes */
        num_bytes = fread( writebuf, sizeof(char), leftover, rdfd );
        if( num_bytes == 0 ){ /* Check for error */
            if( ferror(rdfd) ){ /* Error */
                printf("Error reading contents of file.\n");
                exit( EXIT_FAILURE );
            }
            if( feof(rdfd) ){ /* End of file */
                printf("Premature end of file indicator.\n");
                exit( EXIT_FAILURE );
            }
        }
        /* Write only the leftover bytes to the file */
        num_bytes = fwrite( writebuf, sizeof(char), leftover, wrfd );
        /* Move the file stream to the start of the next block */
        if( -1 == fseek( rdfd, (BLOCK_SIZE - leftover), SEEK_CUR ) ){
            perror("fseek"); /* Check return value */
            exit( EXIT_FAILURE ); /* Exit if not moving to the right spot
                in the stream, no longer reading from correct spot */
        }
    }
}
 
