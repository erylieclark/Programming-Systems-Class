/*******************************************************************************
* Filename: fork_pipe.c
*
* Description: 
* Author: Erin Rylie Clark
*
*******************************************************************************/

/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Local Header Files */
#include "fork_pipe.h"

/*------------------------------------------------------------------------------
* Function: copy_and_exec
*
* Description: main directs all of the other functions to perform in the 
*-----------------------------------------------------------------------------*/
void copy_and_exec( int num ){
    int num_bytes;
    char cmd[MAX_CMD_LENGTH];
    /* Now copy over anything from the stdout to the next write */
    while( (num_bytes = read(STDIN_FILENO, cmd, SIZE)) > 0 ){
        if( write(STDOUT_FILENO, cmd, num_bytes ) == -1 ){
            perror("write");
            exit( EXIT_FAILURE );
        }
    }
    printf("This is child %d (process %d)\n", num ,getpid());
    exit( EXIT_SUCCESS );
}

/*------------------------------------------------------------------------------
* Function: fork_pipe
*
* Description: main directs all of the other functions to perform in the 
*-----------------------------------------------------------------------------*/
void fork_pipe( void ){

    int i, wstatus, prev_p[2], next_p[2];
    int num_child = 4;
    pid_t child;

    /* Get the first pipe from the parent process */
    if( pipe(prev_p) ){
        perror("prev and next pipes");
        exit( EXIT_FAILURE );
    }
    if( (write( prev_p[WRITE], MSG, strlen(MSG) )) < 0 ){
        perror("write");
        exit( EXIT_FAILURE );
    }
    
    /* fork the new child process */
    for( i = 0 ; i < num_child ; i++ ){ 
        /* Make the next pipe */
        if( pipe(next_p) ){
            perror("prev and next pipes");
            exit( EXIT_FAILURE );
        }
        /* Make a child process */
        if( (child = fork()) == -1 ){ /* Check return value of fork */
            perror("fork new child");
            exit( EXIT_FAILURE );
        }
        if( !child ){ /* This is the child */
            /* Copy over the file descriptors */
            if( dup2( prev_p[READ], STDIN_FILENO ) == -1){
                perror("dup2 - read to stdin");
                exit( EXIT_FAILURE );
            }
            /* Close those fds so the child process stops waiting for write */
            close( prev_p[WRITE] );
            close( prev_p[READ] );
            /* Check if this is the last stage or not, dup to stdout if no */
            if( i < (num_child - 1) ){ /* If not the last stage */
                if( dup2( next_p[WRITE], STDOUT_FILENO ) == -1){
                    perror("dup2 - write to stdout");
                    exit( EXIT_FAILURE );
                }
                /* Close the file descriptors */
                close( next_p[READ] );
                close( next_p[WRITE] );
            }
            copy_and_exec( i );

        }
        else{ /* This is the parent */
            /* Close the file descriptors so the process can stop */
            close( prev_p[WRITE] );
            close( prev_p[READ] );
            
            /* Now wait for the child to be done */
            if( wait(&wstatus) == -1 ){
                perror("wait");
                exit( EXIT_FAILURE );
            }
            else if( !WIFEXITED(wstatus) ){
                exit( WEXITSTATUS(wstatus) );
            }

            /* Make the next pipe the old pipe */
            prev_p[READ] = next_p[READ];
            prev_p[WRITE] = next_p[WRITE];
            
        }
    }
}










