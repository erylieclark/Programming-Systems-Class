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
/* Local Header Files */
#include "parseline.h"
#include "fork_pipe.h"

/*------------------------------------------------------------------------------
* Function: 
*
* Description:  
*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
* Function: main
*
* Description:  
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){

    /* First check for arguments */
    if( argc > 1 ){ /* An argument was given with mush */
        printf("Argument given.\n");
    }

    while( 1 ){
        printf("8-P ");
        fflush(stdout);
        parseline();
            /* REMINDER: IF NO INPUT GIVEN, SKIP FORKING! */
        fork_pipe();
        cleanup();
    }


    return 0;
}
