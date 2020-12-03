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
* Description:  
*-----------------------------------------------------------------------------*/
void copy_and_exec( stage_t *cur_pntr, int infd, int outfd ){
    int num_bytes;
    char cmd[MAX_CMD_LENGTH];
    int c;
      /* Now copy over anything from the stdout to the next write */
  /*  while( (num_bytes = read(infd, cmd, SIZE)) > 0 ){
        if( write(outfd, cmd, num_bytes ) == -1 ){
            perror("write");
            exit( EXIT_FAILURE );
        }
        printf("writing now...\n");
        fflush(stdout);
    }*/

    while( ( EOF != (c = getchar()))){
        putchar(c);
    }

    execvp( cur_pntr -> command, cur_pntr -> args_v );
    perror(cur_pntr -> command);
    exit( EXIT_FAILURE ); /* Failed if comes back from exec */
}
/*------------------------------------------------------------------------------
* Function: set_child_fd 
*
* Description:  
*-----------------------------------------------------------------------------*/
void set_child_fd( stage_t *cur_pntr, int prev_p[], int next_p[] ){
    int c, infd, outfd;
        /* Check what kind of input this stage has */
    switch( cur_pntr -> input_type ){
        case FILENAME:
            if( (infd = open(cur_pntr -> input, O_RDONLY )) == -1 ){
                perror("open infd");
                exit( EXIT_FAILURE );
            }
            if( dup2( infd, STDIN_FILENO ) == -1){
                perror("dup2 - read");
                exit( EXIT_FAILURE );
            }
            break;
        case PIPE_FROM_STAGE:
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
            if( (outfd = open(cur_pntr -> output,
                ( O_WRONLY | O_CREAT | O_TRUNC ),
                ( S_IRUSR | S_IWUSR )) ) == -1 ){
                exit( EXIT_FAILURE );
            }
            if( dup2( outfd, STDOUT_FILENO ) == -1){
                perror("dup2 - write");
                exit( EXIT_FAILURE );
            }
            close( outfd );
            break;
        case PIPE_TO_STAGE: 
            if( dup2( next_p[WRITE], STDOUT_FILENO ) == -1){
                perror("dup2 - write");
                exit( EXIT_FAILURE );
            }
            break;
        default: /* ORIG_STDOUT */
            /* Do not change from parent */
            break;
    }
    close( prev_p[WRITE] );
    close( prev_p[READ] );
    close( next_p[READ] );
    close( next_p[WRITE] );
    
    execvp( cur_pntr -> command, cur_pntr -> args_v );
    perror(cur_pntr -> command);
    exit( EXIT_FAILURE ); /* Failed if comes back from exec */
}
/*------------------------------------------------------------------------------
* Function: verify_cmds 
*
* Description:  
*-----------------------------------------------------------------------------*/
int verify_cmds( void ){
    stage_t *cur_pntr = head_pntr;
    struct stat buf;
    do{
        if( lstat( cur_pntr -> command, &buf ) ){
            perror( cur_pntr -> command );
            return -1;
        }
    } while( (cur_pntr = cur_pntr -> next_stage) != NULL );
    return 0;
}
/*------------------------------------------------------------------------------
* Function: fork_pipe
*
* Description:  
*-----------------------------------------------------------------------------*/
void fork_pipe( void ){

    int wstatus, prev_p[2], next_p[2];
    int num = 0;
    pid_t child;
    stage_t *cur_pntr = head_pntr;
    sigset_t block_mask;
    sigset_t ublock_mask;
    sigemptyset(&ublock_mask);
    sigemptyset(&block_mask);
    sigaddset( &block_mask, SIGINT );

    /* head_pntr should be null if the command was cd, so skip forking it */
        /* cd is taken care of in parseline.c and is cleaned up after there */
        /* This can also act as a safety for accessing a null pointer */
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
        sigprocmask( SIG_SETMASK, &block_mask, NULL );
        if( (child = fork()) == -1 ){ /* Check return value of fork */
            perror("fork new child");
            exit( EXIT_FAILURE );
        }
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
    }while( (cur_pntr = cur_pntr -> next_stage) != NULL );
            
    /* Now wait for the child to be done */
    while( num-- ){ /* Count down the processes until none left */
        sigprocmask( SIG_SETMASK, &block_mask, NULL );
        if( wait(&wstatus) == -1 ){
            perror("wait");
            exit( EXIT_FAILURE );
        }
        sigprocmask( SIG_SETMASK, &ublock_mask, NULL );
        if( !WIFEXITED(wstatus) ){ /* terminated abnormally */
            if( sig_flag ){
                if( num ){
                    sig_flag = 0;
                    continue;
                }
                else{
                    sig_flag = 0;
                    return;
                }
            }
        }
        else{ /* Terminated normally */
            if( WEXITSTATUS(wstatus) != 0 ){ /* Nonzero exit status*/
                cleanup();
                return;
            }
        }
    }
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
    head_pntr = NULL; /* Reset head_pntr */
}






