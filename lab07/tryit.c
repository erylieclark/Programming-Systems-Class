/*******************************************************************************
* tryit.c
*
* Description: This function takes in a path to a program, creates a child
*   process, executes the program, and reports on the exit status of the child
*   process. 
*
* Author: Erin Rylie Clark
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    pid_t child;
    int wstatus;
    struct stat buf;

    /* Check the input */
    if( argc != 2 ){
        fprintf(stderr, "usage: tryit command\n");
        exit( EXIT_FAILURE );
    }
    /* Check if the file is valid */
    if( stat(argv[1], &buf) == -1 ){
        perror(argv[1]);
        fprintf(stderr, "Process %d exited with an error value.\n", getpid());
        exit( EXIT_FAILURE );
    }

    child = fork(); /* Create the new process */

    if( child == -1 ){ /* Check the return value for errors */
        perror("fork");
        exit( EXIT_FAILURE );
    }
    else if( !child ){ /* Exec the path in the child */
        execlp( argv[1], argv[1], NULL );
        perror("exec");
        exit( EXIT_FAILURE );
    }

    /* Now wait for the child */
    if( wait(&wstatus) == -1 ){
        perror("wait");
        exit( EXIT_FAILURE );
    }
    else if( WIFEXITED(wstatus) ){
        if( WEXITSTATUS(wstatus) == 0 ){
            printf("Process %d succeeded.\n", child );
            exit( WEXITSTATUS(wstatus) );
        }
        else{
            printf("Process %d exited with an error value.\n", child );
            exit( WEXITSTATUS(wstatus) );
        }
    }
    else if( WIFSIGNALED(wstatus) ){
        printf("Process %d was terminated by a signal.\n", child);
        exit( EXIT_FAILURE );
    }
    return 0;
}







