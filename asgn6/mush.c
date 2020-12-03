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
static int caught;
int sig_flag = 0;
/* Local Header Files */
#include "parseline.h"
#include "fork_pipe.h"
/*------------------------------------------------------------------------------
* Function: 
*
* Description:  
*-----------------------------------------------------------------------------*/
void sig_handler( int signum ){
    sig_flag = 1;
}
/*------------------------------------------------------------------------------
* Function: main
*
* Description:  
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    struct sigaction sa;
    /* Set up the sigint handler */
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;
    sigset_t block_mask;
    sigset_t ublock_mask;

    sigemptyset(&sa.sa_mask);
    sigemptyset(&ublock_mask);
    sigemptyset(&block_mask);
    sigaddset( &block_mask, SIGINT );

    if( -1 == sigaction( SIGINT, &sa, NULL )){
        perror("sigaction");
        exit( EXIT_FAILURE );
    }

    /* First check for arguments */
    if( argc > 1 ){ /* An argument was given with mush */
        printf("Argument given.\n");
    }

    while( 1 ){
        printf("8-P ");
        fflush(stdout);
        if( parseline() ){ /* -1 on failure cleans up all malloc'd mem */
            continue; /* Skip working with this command */
        }
        fork_pipe(); 
        fflush( stdout ); /* In case there is something left */
        cleanup();
    }
    return 0;
}
