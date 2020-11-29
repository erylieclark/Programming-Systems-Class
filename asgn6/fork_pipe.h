/*******************************************************************************
* File: fork_pipe.h
*
* Description:
* 
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _FORK_PIPE_H 
#define _FORK_PIPE_H

/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* Read and write */
#include <sys/types.h>
#include <sys/wait.h>

/* Local Header Files */
#include "store_stage_info.h"

/* Defines */
#define SIZE 1024
#define READ 0
#define WRITE 1
#define MSG "Hello\n"

void copy_and_exec( int num );
void fork_pipe( void );

#endif /* _FORK_PIPE_H */

