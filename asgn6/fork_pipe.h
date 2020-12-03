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
#include <sys/stat.h> /* open */
#include <sys/wait.h>
#include <fcntl.h>

/* Local Header Files */
#include "store_stage_info.h"
#include "parseline.h"

/* Defines */
#define SIZE 1024
#define READ 0
#define WRITE 1

void copy_and_exec( stage_t *cur_pntr, int infd, int outfd );
void set_child_fd( stage_t *cur_pntr, int prev_p[], int next_p[] );
int verify_cmds( void );
void fork_pipe( void );
void cleanup( void );

#endif /* _FORK_PIPE_H */

