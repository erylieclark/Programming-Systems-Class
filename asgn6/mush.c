/*******************************************************************************
* Filename: mush.c
*
* Description: mush is a "minimally useful shell" that will...
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/* Global Variable */
int sig_flag = 0; /* Used for signal handler */

/* Local Header Files */
#include "parseline.h"
#include "fork_pipe.h"

/*------------------------------------------------------------------------------
* Function: sig_handler
*
* Description: sig_handler is called on SIGINT and simply raises the sig flag
*   so that other functions will be able to handle it properly
*-----------------------------------------------------------------------------*/
void sig_handler( int signum ){
    sig_flag = 1;
}
/*------------------------------------------------------------------------------
* Function: main
*
* Description: main sets up the signal handler, checks if the shell is being
*   run with arguments or not, and then calls the functions for parsing the 
*   command, forking and executing the child processes, and then helps clean
*   up. 
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    struct sigaction sa;
    
    /* Set up the sigint handler */
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if( -1 == sigaction( SIGINT, &sa, NULL )){
        perror("sigaction");
        exit( EXIT_FAILURE );
    }

    while( 1 ){
        /* Prompt only if reading and writing to a terminal */
        if( isatty( STDIN_FILENO ) && isatty( STDOUT_FILENO ) ){
            printf("8-P ");
            fflush(stdout); /* Make sure the prompt gets written immediately */
        }
        if( parseline() ){ /* -1 on failure cleans up all malloc'd mem */
            continue; /* Skip working with this command */
        }
        fork_pipe(); /* Fork and execute */ 
        fflush( stdout ); /* In case there is something left */
        cleanup(); /* Free all malloc'd memory */
    }
    return 0;
}
