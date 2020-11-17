/*******************************************************************************
* forkit.c
*
* Description: This program simply creates a child program, prints out its
*   process id, and exits.
*
* Author: Erin Rylie Clark
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    pid_t child;
    int wstatus;

    printf("Hello, world!\n");

    child = fork(); /* Create the new process */

    if( child == -1 ){ /* Check the return value for errors */
        perror("fork");
        exit( EXIT_FAILURE );
    }
    else if( !child ){ /* This is the child */
        printf( "This is the child, pid %d.\n", getpid() );
        exit( EXIT_FAILURE );
    }
    else{ /* This is the parent */
        printf( "This is the parent, pid %d.\n", getpid() );
    }

    /* Now wait for the child */
    if( wait(&wstatus) == -1 ){
        perror("wait");
        exit( EXIT_FAILURE );
    }
    else if( !WIFEXITED(wstatus) ){
        exit( WEXITSTATUS(wstatus) );
    }

    /* Say goodbye */
    printf( "This is the parent, pid %d, signing off.\n", getpid() );

    return 0;
}







