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
void copy_and_exec( stage_t *cur_pntr, int infd, int outfd ){
    int num_bytes;
    char cmd[MAX_CMD_LENGTH];
    /* Now copy over anything from the stdout to the next write */
    while( (num_bytes = read(infd, cmd, SIZE)) > 0 ){
        if( write(outfd, cmd, num_bytes ) == -1 ){
            perror("write");
            exit( EXIT_FAILURE );
        }
    }
    execvp( cur_pntr -> command, cur_pntr -> args_v );
    perror("exec");
    exit( EXIT_FAILURE ); /* Failed if comes back from exec */
}
/*------------------------------------------------------------------------------
* Function: set_child_fd 
*
* Description:  
*-----------------------------------------------------------------------------*/
void set_child_fd( stage_t *cur_pntr, int prev_p[], int next_p[] ){
    int infd, outfd;

    /* Check what kind of input this stage has */
    switch( cur_pntr -> input_type ){
        case FILENAME:
            infd = open(cur_pntr -> input, O_RDONLY );
            break;
        default: /* Covers ORIG_STDIN and PIPE_FROM_STAGE (will be same) */
            infd = STDIN_FILENO;
            break;
    }

    /* Copy over the file descriptors */
    if( dup2( prev_p[READ], infd ) == -1){
        perror("dup2 - read");
        exit( EXIT_FAILURE );
    }
    /* Close those fds so the child process stops waiting for write */
    close( prev_p[WRITE] );
    close( prev_p[READ] );
    
    /* Check what kind of output this is */
    switch( cur_pntr -> output_type ){
        case FILENAME:
            outfd = open(cur_pntr -> output, ( O_WRONLY | O_CREAT | O_TRUNC ),
                ( S_IRUSR | S_IWUSR ));
            break;
        default: /* Covers ORIG_STDOUT and PIPE_TO_STAGE (will be same) */
            outfd = STDOUT_FILENO;
            break;
    }

    /* Check if this is the last stage or not, dup to stdout if no */
    if( (cur_pntr -> next_stage) != NULL ){ /* If not the last stage */
        if( dup2( next_p[WRITE], outfd ) == -1){
            perror("dup2 - write");
            exit( EXIT_FAILURE );
        }
        /* Close the file descriptors */
        close( next_p[READ] );
        close( next_p[WRITE] );
    }
    copy_and_exec( cur_pntr, infd, outfd ); /* Wont come back */
}
/*------------------------------------------------------------------------------
* Function: fork_pipe
*
* Description:  
*-----------------------------------------------------------------------------*/
void fork_pipe( void ){

    int wstatus, prev_p[2], next_p[2];
    int i = 0;
    pid_t child;
    stage_t *cur_pntr = head_pntr;
    /* Get the first pipe from the parent process */
    if( pipe(prev_p) ){
        perror("prev and next pipes");
        exit( EXIT_FAILURE );
    }
    /* Cyle through the stages and fork the processes */
    do{ 
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
            set_child_fd( cur_pntr, prev_p, next_p );
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
        i++;
    }while( (cur_pntr = cur_pntr -> next_stage) != NULL );
}


/*------------------------------------------------------------------------------
* Function: cleanup
*
* Description: this function is made to free all of the memory that was
*   allocated in the parseline portion of this program. This will also likely
*   be called when ^C is pressed 
*-----------------------------------------------------------------------------*/
void cleanup( void ){
    stage_t *cur_pntr = head_pntr;
    stage_t *temp;
    int i;
    if( cur_pntr == NULL ){
        return; /* if no structs have been made */
    }
    do{
        i = 0; 
        free( cur_pntr -> full_command );
        free( cur_pntr -> command );
        if( cur_pntr -> input ){
            free( cur_pntr -> input );
        }
        if( cur_pntr -> output ){
            free( cur_pntr -> output );
        }
        while( i < (cur_pntr -> num_args) ){
            free( cur_pntr -> args_v[i] );
            i++;
        }
        temp = cur_pntr;
        cur_pntr = cur_pntr -> next_stage;
        free( temp );
    }while( cur_pntr != NULL ); 
}






