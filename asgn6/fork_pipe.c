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
* Function: set_child_fd 
*
* Description: this function is only called within the child process and is
*   used to set up the pipes/input/output appropriately. It uses the linked
*   list of structures that contain the information for each stage. It will
*   then close all unnecessary file descriptors and exec the command from the
*   stage.
*
* param: cur_pntr - the pointer to the current stage struct
* param: prev_p - array containing file descriptors to the preceding pipe
* param: next_p - array containing file descriptors for the program to write to
*-----------------------------------------------------------------------------*/
void set_child_fd( stage_t *cur_pntr, int prev_p[], int next_p[] ){
    int infd, outfd;
    
    /* Check what kind of input this stage has */
    switch( cur_pntr -> input_type ){
        case FILENAME:
            /* Open the file as read only and get its file descriptor */
            if( (infd = open(cur_pntr -> input, O_RDONLY )) == -1 ){
                perror("open infd");
                exit( EXIT_FAILURE );
            }
            /* If input is coming from a file, set the fd of the file
                descriptor so that the program can read from stdin */
            if( dup2( infd, STDIN_FILENO ) == -1){
                perror("dup2 - read");
                exit( EXIT_FAILURE );
            }
            close( infd );
            break;
        case PIPE_FROM_STAGE:
            /* Connect the pipe to STDIN if its coming from a pipe */
            if( dup2( prev_p[READ], STDIN_FILENO ) == -1){
                perror("dup2 - read");
                exit( EXIT_FAILURE );
            }
            break;
        default: /* ORIG_STDIN */
            /* Do not change from parent */ 
            break;
    }

    /* Check what kind of output this is */
    switch( cur_pntr -> output_type ){
        case FILENAME:
            /* Open with user read write permissions and get the fd */
            if( (outfd = open(cur_pntr -> output,
                ( O_WRONLY | O_CREAT | O_TRUNC ),
                ( S_IRUSR | S_IWUSR )) ) == -1 ){
                exit( EXIT_FAILURE );
            }
            /* If input is going to the file, set the fd so that the progrm
                can write to stdout */
            if( dup2( outfd, STDOUT_FILENO ) == -1){
                perror("dup2 - write");
                exit( EXIT_FAILURE );
            }
            close( outfd );
            break;
        case PIPE_TO_STAGE:
            /* Connect the pipe to STDOUT if going to a pipe */ 
            if( dup2( next_p[WRITE], STDOUT_FILENO ) == -1){
                perror("dup2 - write");
                exit( EXIT_FAILURE );
            }
            break;
        default: /* ORIG_STDOUT */
            /* Do not change from parent */
            break;
    }
    /* Close all file descriptors from the child */
    close( prev_p[WRITE] );
    close( prev_p[READ] );
    close( next_p[READ] );
    close( next_p[WRITE] );
    /* Exec the program with the argument array from the struct */
    execvp( cur_pntr -> command, cur_pntr -> args_v );
    perror(cur_pntr -> command);
    exit( EXIT_FAILURE ); /* Failed if comes back from exec */
}
/*------------------------------------------------------------------------------
* Function: fork_pipe
*
* Description: fork_pipe initiates the pipes, forks off the child, and waits
*   for the children to return. It blocks both fork and wait from receiving
*   a SIGINT in the middle of them, and handles a SIGINT properly afterwards 
*-----------------------------------------------------------------------------*/
void fork_pipe( void ){
    int wstatus, prev_p[2], next_p[2];
    int num = 0; /* Keep track of processes forked */
    pid_t child;
    stage_t *cur_pntr = head_pntr;
    
    /* Masks used for blocking out sigint for functions
        that can't be interupted */
    sigset_t block_mask;
    sigset_t ublock_mask;
    sigemptyset(&ublock_mask);
    sigemptyset(&block_mask);
    sigaddset( &block_mask, SIGINT );

    /* First check if there are any stages to exec */
        /* This will be true for a cd and potentially some parseline
            circumstances */
    if( head_pntr == NULL){
        return; /* Do nothing if no commands */
    }

    /* Get the first pipe from the parent process */
    if( pipe(prev_p) ){
        perror("prev pipe");
        exit( EXIT_FAILURE );
    }
    /* Cyle through the stages and fork the processes */
    do{ 
        /* Make the next pipe */
        if( pipe(next_p) ){
            perror("next pipe");
            exit( EXIT_FAILURE );
        }
        /* Make a child process */
        num++; /* Count the processes being forked */
        /* Block SIGINT while we fork the process */
        sigprocmask( SIG_SETMASK, &block_mask, NULL );
        if( (child = fork()) == -1 ){ /* Check return value of fork */
            perror("fork new child");
            exit( EXIT_FAILURE );
        }
        /* Unblock it immediately after forking for both child and parent */
        sigprocmask( SIG_SETMASK, &ublock_mask, NULL );
        if( !child ){ /* This is the child */
            set_child_fd( cur_pntr, prev_p, next_p );
        }
        else{ /* This is the parent */
            /* Close the file descriptors so the process can stop */
            close( prev_p[WRITE] );
            close( prev_p[READ] );
            /* Make the next pipe the old pipe */
            prev_p[READ] = next_p[READ];
            prev_p[WRITE] = next_p[WRITE];
        }
        /* Check if there is a next stage, and repeat if yes */
    }while( (cur_pntr = cur_pntr -> next_stage) != NULL );
            
    /* Now wait for the child to be done */
    while( num-- ){ /* Count down the processes until none left */
        /* Do not let wait be interrupted by SIGINT */
        sigprocmask( SIG_SETMASK, &block_mask, NULL );
        if( wait(&wstatus) == -1 ){
            perror("wait");
            exit( EXIT_FAILURE );
        }
        /* Unblock immediately after */
        sigprocmask( SIG_SETMASK, &ublock_mask, NULL );
        /* Check if it terminated abnormally, ie SIGINT, etc */
        if( !WIFEXITED(wstatus) ){
            if( sig_flag ){ /* Check if it was SIGINT */
                if( num ){ /* If there are still processes left to wait for */
                    sig_flag = 0; /* Reset the flag for the next processes */
                    continue; /* Now wait for the next one */
                }
                else{ /* If this is the last process to wait for */
                    sig_flag = 0; /* Reset flag and return to main */
                    return; /* main will clean up after these processes */
                }
            }
        }
    }
}
/*------------------------------------------------------------------------------
* Function: cleanup
*
* Description: this function is made to free all of the memory that was
*   allocated in the parseline portion of this program..
*-----------------------------------------------------------------------------*/
void cleanup( void ){
    stage_t *cur_pntr = head_pntr;
    stage_t *temp;
    int i;
    if( cur_pntr == NULL ){
        return; /* if no structs have been made */
    }
    do{ /* Free all of the strings that were malloc'd */
        i = 0; 
        free( cur_pntr -> full_command );
        free( cur_pntr -> command );
        /* Only free input and output filenames if they were set */
        if( cur_pntr -> input ){
            free( cur_pntr -> input );
        }
        if( cur_pntr -> output ){
            free( cur_pntr -> output );
        }
        /* Free all of the arguments in the list */
        while( i < (cur_pntr -> num_args) ){
            free( cur_pntr -> args_v[i] );
            i++;
        }
        temp = cur_pntr;
        cur_pntr = cur_pntr -> next_stage;
        free( temp ); /* Free the structure */
    }while( cur_pntr != NULL ); 
    head_pntr = NULL; /* Reset head_pntr */
}

